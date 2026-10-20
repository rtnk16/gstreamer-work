#include <gst/gst.h>

int main(int argc, char **argv) {
    gst_init(&argc, &argv);
    if (argc > 2) {
        g_printerr("Usage: %s [output.png]\n", argv[0]);
        return 2;
    }
    const char *output = argc == 2 ? argv[1] : "snapshot.png";
    GError *error = NULL;
    GstElement *pipeline = gst_parse_launch(
        "videotestsrc num-buffers=1 pattern=smpte ! "
        "video/x-raw,width=640,height=360 ! videoconvert ! "
        "pngenc ! filesink name=output", &error);
    if (error) {
        g_printerr("Pipeline error: %s\n", error->message);
        g_clear_error(&error);
        if (pipeline) gst_object_unref(pipeline);
        return 1;
    }
    GstElement *sink = gst_bin_get_by_name(GST_BIN(pipeline), "output");
    g_object_set(sink, "location", output, NULL);
    gst_object_unref(sink);
    GstBus *bus = gst_element_get_bus(pipeline);
    int result = 1;
    if (gst_element_set_state(pipeline, GST_STATE_PLAYING) != GST_STATE_CHANGE_FAILURE) {
        GstMessage *message = gst_bus_timed_pop_filtered(
            bus, 10 * GST_SECOND, GST_MESSAGE_EOS | GST_MESSAGE_ERROR);
        if (!message) {
            g_printerr("Timed out waiting for the image.\n");
        } else {
            if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_EOS) {
                g_print("Saved 640x360 test image: %s\n", output);
                result = 0;
            } else {
                gst_message_parse_error(message, &error, NULL);
                g_printerr("GStreamer error: %s\n", error->message);
                g_clear_error(&error);
            }
            gst_message_unref(message);
        }
    } else {
        g_printerr("Could not start the pipeline.\n");
    }
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(bus);
    gst_object_unref(pipeline);
    return result;
}
