//
// entry point to TensorFlow
//

#ifndef ELOQUENTTINYML_TENSORFLOW_H
#define ELOQUENTTINYML_TENSORFLOW_H

#include <Arduino.h>
#include <math.h>

// ESP32 may define max as a macro and this creates problems
// un-define, then re-define at the end
#ifdef max
#define REDEFINE_MAX
#undef max
#undef min
#endif

#ifdef DEFAULT
#define REDEFINE_DEFAULT
#undef DEFAULT
#endif

#define ELOQUENT_TINYML_ARM 1
#include "./tensorflow/arm/tensorflow/lite/version.h"
#include "./tensorflow/arm/tensorflow/lite/schema/schema_generated.h"
#include "./tensorflow/arm/tensorflow/lite/micro/micro_error_reporter.h"
#include "./tensorflow/arm/tensorflow/lite/micro/all_ops_resolver.h"
#include "./tensorflow/arm/tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "./tensorflow/arm/tensorflow/lite/micro/micro_interpreter.h"
#include "./tensorflow/arm/patches/AllOpsResolver.h"
#include "./tensorflow/arm/patches/MicroMutableOpResolver.h"

// common files
#include "./tensorflow/common/AbstractTensorFlow.h"
#include "./tensorflow/common/AllOpsTensorFlow.h"
#include "./tensorflow/common/MutableTensorFlow.h"


#ifdef REDEFINE_MAX
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifdef REDEFINE_DEFAULT
#define DEFAULT 1
#endif

#endif //ELOQUENTTINYML_TENSORFLOW_H
