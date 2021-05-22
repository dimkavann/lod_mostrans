#include <curl/curl.h>
// функция, вызываемая cURL для записи полученых данных
std::string curlBuffer;
size_t curlWriteFunc(char *data, size_t size, size_t nmemb, std::string *buffer)  {  
    size_t result = 0;  
    if (buffer != NULL){  
        buffer->append(data, size * nmemb);  
        result = size * nmemb;  
    }  
    return result;  
}  

void ajax(string url, string params){
    // запрашиваемая страничка(путь до login screen)
    char *url = url;
    // передаваемые параметры
    char *urlPOST = params;
    // буфер для сохранения текстовых ошибок
    char curlErrorBuffer[CURL_ERROR_SIZE];

    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlErrorBuffer);
        // задаем URL...
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // переходить по "Location:" указаному в HTTP заголовке  
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        // не проверять сертификат удаленного сервера
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, O);
        // использовать метод POST для отправки данных
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        // параметры POST
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, urlPOST);
        // функция, вызываемая cURL для записи полученых данных
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteFunc);

        // выполнить запрос
        CURLcode curlResult = curl_easy_perform(curl);
        // завершение сеанса
        curl_easy_cleanup(curl);

        if(curlResult == CURLE_OK){
            std::cout << curlBuffer << std::endl;
            return(0);
        }else{
            std::cout << "Ошибка(" << curlResult << "): " << curlErrorBuffer << std::endl;
            return(-1);
        }
    
    }
}