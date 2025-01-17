#ifndef LITE_AI_ORT_CORE_ORT_HANDLER_H
#define LITE_AI_ORT_CORE_ORT_HANDLER_H


#include <onnxruntime_cxx_api.h>
#include <opencv.hpp>

// global
namespace onnxlib
{
    // single input & multi outputs. not support for dynamic shape currently.
    class BasicOrtHandler
    {
    protected:
        Ort::Env ort_env;
        Ort::Session* ort_session = nullptr;
        const char* input_name = nullptr;

        //GetInputNameAllocated返回的字符串指针指向的数据自动释放，使用inputNodeNameAllocatedStrings保存指针
        std::vector<Ort::AllocatedStringPtr> inputNodeNameAllocatedStrings;
        std::vector<const char*> input_node_names;
        std::vector<int64_t> input_node_dims; // 1 input only.
        std::size_t input_tensor_size = 1;
        std::vector<float> input_values_handler;
        Ort::MemoryInfo memory_info_handler = Ort::MemoryInfo::CreateCpu(
            OrtArenaAllocator, OrtMemTypeDefault);
        std::vector<Ort::AllocatedStringPtr> outputNodeNameAllocatedStrings;
        std::vector<const char*> output_node_names;
        std::vector<std::vector<int64_t>> output_node_dims; // >=1 outputs
        std::string onnx_path;
        const char* log_id = nullptr;
        int num_outputs = 1;

    protected:
        const unsigned int num_threads; // initialize at runtime.

    protected:
        explicit BasicOrtHandler(const std::string& _onnx_path, unsigned int _num_threads = 1);

        virtual ~BasicOrtHandler();

        // un-copyable
    protected:
        BasicOrtHandler(const BasicOrtHandler&) = delete;

        BasicOrtHandler(BasicOrtHandler&&) = delete;

        BasicOrtHandler& operator=(const BasicOrtHandler&) = delete;

        BasicOrtHandler& operator=(BasicOrtHandler&&) = delete;

    protected:
        virtual Ort::Value transform(const cv::Mat& mat) = 0;

    private:
        void initialize_handler();

        void print_debug_string();
    };

    // multi inputs & multi outputs. not support for dynamic shape currently.
    class BasicMultiOrtHandler
    {
    protected:
        Ort::Env ort_env;
        Ort::Session* ort_session = nullptr;

        //GetInputNameAllocated返回的字符串指针指向的数据自动释放，使用inputNodeNameAllocatedStrings保存指针
        std::vector<Ort::AllocatedStringPtr> inputNodeNameAllocatedStrings;
        std::vector<const char*> input_node_names;
        std::vector<std::vector<int64_t>> input_node_dims; // >=1 inputs.
        std::vector<size_t> input_tensor_sizes;
        std::vector<std::vector<float>> input_values_handlers; // multi handlers.
        Ort::MemoryInfo memory_info_handler = Ort::MemoryInfo::CreateCpu(
            OrtArenaAllocator, OrtMemTypeDefault);
        std::vector<Ort::AllocatedStringPtr> outputNodeNameAllocatedStrings;
        std::vector<const char*> output_node_names;
        std::vector<std::vector<int64_t>> output_node_dims; // >=1 outputs
        std::string onnx_path;
        const char* log_id = nullptr;
        int num_outputs = 1;
        int num_inputs = 1;

    protected:
        const unsigned int num_threads; // initialize at runtime.

    protected:
        explicit BasicMultiOrtHandler(const std::string& _onnx_path, unsigned int _num_threads = 1);

        virtual ~BasicMultiOrtHandler();

        // un-copyable
    protected:
        BasicMultiOrtHandler(const BasicMultiOrtHandler&) = delete;

        BasicMultiOrtHandler(BasicMultiOrtHandler&&) = delete;

        BasicMultiOrtHandler& operator=(const BasicMultiOrtHandler&) = delete;

        BasicMultiOrtHandler& operator=(BasicMultiOrtHandler&&) = delete;

    protected:
        virtual std::vector<Ort::Value> transform(const std::vector<cv::Mat>& mats) = 0;

    private:
        void initialize_handler();

        void print_debug_string();
    };
}

#endif //LITE_AI_ORT_CORE_ORT_HANDLER_H