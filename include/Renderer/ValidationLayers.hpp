#include <vector>


namespace EngineRenderer {
    class ValidationLayers{
        public:
            bool CheckValidationSupport();
           
            const std::vector<const char*> validationLaye{
                "VK_LAYER_KHRONOS_validation"
            };
    };
}