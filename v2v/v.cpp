// Транспорт
class TrafficHook::vehicle : public Logic {
    // ID светофора
    private : int id;
    // Координаты (динамичны)
    private : float x;
    private : float y;
    private : float angle;
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