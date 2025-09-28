

class ObjectManager{
    static ObjectManager& get(){
        static ObjectManager instance;
        return instance;
    }
};