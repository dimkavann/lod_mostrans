// Перекресток (светофор)
class TrafficHook::infrastructure{
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
};