#include <vector>


namespace renderer {
    class ValidationLaye{
        public:
            bool CheckValidationSupport();
           
            const std::vector<const char*> validationLaye{
                "VK_LAYER_KHRONOS_validation"
            };
    };
}