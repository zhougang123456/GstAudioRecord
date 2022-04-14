//
// Created by zhougang on 2022/4/13.
//

#include "audioRecord.h"
#include <jni.h>
#include "gst/gst.h"

static void my_log_func(GstDebugCategory* category,
    GstDebugLevel level,
    const gchar* file,
    const gchar* function,
    gint line,
    GObject* object,
    GstDebugMessage* message,
    gpointer user_data) {

    g_info("MyLogFunc: [Level:%d] %s:%s:%d  %s\n",
        level, file, function, line,
        gst_debug_message_get(message));

}

static gboolean record_bus_cb(GstBus *bus, GstMessage *msg, gpointer data)
{
    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_ERROR: {
            GError* err = NULL;
            gchar* debug_info = NULL;
            gst_message_parse_error(msg, &err, &debug_info);
            g_warning("[RECORD]GStreamer error from element %s: %s",
                      GST_OBJECT_NAME(msg->src), err->message);
            if (debug_info) {
                g_warning("[RECORD]debug information: %s", debug_info);
                g_free(debug_info);
            }
            g_clear_error(&err);
            break;
        }
        default:
            break;
    }
    return TRUE;
}

GstElement *pipeline = NULL;
extern "C" {
JNIEXPORT void
JNICALL Java_com_zhougang_gstaudiorecord_MainActivity_NativeRecordStart(JNIEnv *env, jclass obj) {

    gst_debug_add_log_function(my_log_func, NULL, NULL);
    gst_debug_set_active(TRUE);
    gst_debug_set_default_threshold(GST_LEVEL_WARNING);

    gchar *desc =
        g_strdup_printf("openslessrc name=audiosrc ! queue ! audioconvert ! audioresample ! openslessink name=audiosink");
    GError *err = NULL;
    g_info("GStreamer pipeline: %s", desc);

    pipeline = gst_parse_launch_full(desc, NULL, GST_PARSE_FLAG_FATAL_ERRORS, &err);
    g_free(desc);

    if (!pipeline || err) {
        g_info("GStreamer error: %s", err->message);
        g_clear_error(&err);
        return;
    }

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    gst_bus_add_watch(bus, record_bus_cb, NULL);
    gst_object_unref(GST_OBJECT(bus));

    if (gst_element_set_state(pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
        g_info("set state failure");
        return;
    }
}

JNIEXPORT void
JNICALL Java_com_zhougang_gstaudiorecord_MainActivity_NativeRecordStop(JNIEnv *env, jclass obj) {
    if (gst_element_set_state(pipeline, GST_STATE_NULL) == GST_STATE_CHANGE_FAILURE) {
        g_info("set state failure");
        return;
    }
}

}