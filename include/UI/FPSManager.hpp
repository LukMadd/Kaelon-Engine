namespace EngineUI{
    class FPSManager{
        public:
            float updateFPS(float deltaTime);

            float smoothFPS(float fps);

        private:
            float fps = 0.0f;
            float frameTimeSum = 0.0f;
            int frameCount = 0;
            const int smoothingWindow = 60; 

            float smoothedFPS = 0.0f;
            const float alpha = 0.008f; //Determines how smoothed the fps counter is(less is more)
    };
}