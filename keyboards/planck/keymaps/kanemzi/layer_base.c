#include "layer_base.h"
#include "layers.h"

layer_info_t layer_info_base =
{
    .is_predominant = true,
    .on_layer_show = NULL,
    .on_layer_hide = NULL,
    .on_layer_render = NULL,
    .on_process_record = NULL
};
