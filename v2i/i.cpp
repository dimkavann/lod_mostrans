class TrafficHook::Logic{
    // Функция логики для машины
    public : void checkLogicForCar(){
        
    }
}

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
    
    // Пропустить N машин (удалить их из очереди)
    public : void openGate(int n){
        this->pool.erase(pool.begin(), pool.begin() + n);
    }

    // Метод отправляющий запрос на светофор (железо)
    public : void sendAjax(){
        ajax('localhost', '{ status: 1 }');
    }
    
    // Функция запуска из вне системы
    public : void trigger(){
        // если есть люди в очереди открываем гейт
        if(this->pool.size() > 0){
            this->openGate(50);
            this->sendAjax();
        }
    }
    
};