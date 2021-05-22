// Транспорт
class TrafficHook::vehicle{
    // структуры
    public : struct car{
        int id;
        float x;
        float y;
        float speed;
    };

    public : struct request{
        int requested_car_id;
        TrafficHook::car host_car_data;
    };

    public : struct response{
        int host_car_id;
        bool status;
    };

    // ID светофора
    private : int id;
    // Координаты (динамичны)
    private : float x;
    private : float y;
    // Id подключенных устройств
    // Проверяется блокчейном
    private : std::vector <int> connections;
    // Машины в сети
    private : std::vector <TrafficHook::car> vehicles;

    // Запросы и ответы
    private : std::vector <TrafficHook::request>  req_pool;
    private : std::vector <TrafficHook::response> res_pool;

    // Конструктор
    construct(){}

    // Глонасс передает сюда данные и тут меняем координаты
    public : void changeCoords(float x, float y){
        this->x = x;
        this->y = y;
    }

    // Глонасс сообщает о машине поблизости
    public : void nearCar(TrafficHook::car obj){
        this->vehicles.push_back(obj);
    }

    // Запрос во вне
    public : void generateConnectRequest(TrafficHook::request){
        this->req_pool.push_back(obj); 
    }

    // Ответ из вне
    public : void getConnectResponse(TrafficHook::response){
        this->req_pool.push_back(res_pool); 
    }

    // Функция запуска из вне системы
    public : void trigger(){}

    
};