
class TrafficHook::Logic{
    // Шаблон деления вектора на подвекторы
    template<typename T> 
    std::vector<std::vector<T>> SplitVector(const std::vector<T>& vec, size_t n){
        std::vector<std::vector<T>> outVec;
        size_t length = vec.size() / n;
        size_t remain = vec.size() % n;
        size_t begin = 0;
        size_t end = 0;

        for (size_t i = 0; i < std::min(n, vec.size()); ++i){
            end += (remain > 0) ? (length + !!(remain--)) : length;
            outVec.push_back(std::vector<T>(vec.begin() + begin, vec.begin() + end));
            begin = end;
        }
        return outVec;
    }


    // структуры
    public : struct car{
        int id;
        float x;
        float y;
        float angel;
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
    
    // Метод отправляющий запрос на светофор (железо)
    public : void sendAjax(){
        ajax('localhost', '{ status: 1 }');
    }

    // Функция логики для машины
    // Запускать только из дочернего класса
    // Какие-то версии плюсов по идее будут ругаться
    public : void checkLogicForCar(std::vector <TrafficHook::car> vehicles, TrafficHook::infrastructure trafficlight){
        if(vehicles.size() == 0){ cout << "Smth wrong else..." << endl; }
        // получаем количество машин для каждого направления
        // из движений на перекрестке      
        // сортируем машины по углам и делим на 4 группы
        std::sort(vehicles.begin(), vehicles.end(), [](const TrafficHook::car &a, const TrafficHook::car &b){ 
            return a.angel < b.angel; 
        });
        // Проще так чем заморачиваться с циклом
        if(vehicles.size() % 4){
            // Бьем вектор на 4 направления и идем вниз
            // Если вектор разбился смотрим где меньше машин те и пускаем
            std::vector <std::vector <TrafficHook::car>> vehicles_split splitVector(vehicles, 4);
            // Сортировочка
            std::sort(vehicles_split.begin(), vehicles_split.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
                return a[2] < b[2];
            });
            this->trigger(vehicles[0]);
        }else{
            if(vehicles.size() % 3){
                // Бьем вектор на 3 направления и идем вниз
                std::vector <std::vector <TrafficHook::car>> vehicles_split splitVector(vehicles, 3);
                // Сортировочка
                std::sort(vehicles_split.begin(), vehicles_split.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
                    return a[2] < b[2];
                });
                this->trigger(vehicles[0);
            }else{
                if(vehicles.size() % 2){
                    // Бьем вектор на 2 и идем вниз
                    std::vector <std::vector <TrafficHook::car>> vehicles_split splitVector(vehicles, 2);
                    // Сортировочка
                    std::sort(vehicles_split.begin(), vehicles_split.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
                        return a[2] < b[2];
                    });
                    this->trigger();
                }else{
                    // Зажигаем светофор для транспортного средства с id N
                    this->trigger(vehicles[0]);
                }
            }
        }
    }
};

// Перекресток (светофор)
class TrafficHook::infrastructure : public Logic{
    // ID светофора
    private : int id;
    // Координаты (статичны)
    private : float x;
    private : float y;
    // Id подключенных устройств
    // Проверяется блокчейном
    private : std::vector <int> connections;
    // Очередь на проезд
    private : std::vector <int> pool;
    // Еще не сделано нужно реализовать принятие из вне
    private : std::vector <TrafficHook::car> vehicles;
    
    // Пропустить N машин (удалить их из очереди)
    public : void openGate(int n){
        this->checkLogicForCar(
            this->vehicles, 
            this->collectThisInfrastructure()
        );
        this->pool.erase(pool.begin(), pool.begin() + n);
    }
    
    // Функция запуска из вне системы
    public : void trigger(){
        // если есть люди в очереди открываем гейт
        if(this->pool.size() > 0){
            this->openGate(50);
            this->sendAjax();
        }
    }

    // 
    TrafficHook::infrastructure collectThisInfrastructure(){
        return TrafficHook::infrastructure object(
            this->id, 
            this->x, 
            this->y 
            this->connections, 
            this->pool
        );
    }
    
    
};