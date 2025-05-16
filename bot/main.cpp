#include <iostream>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <sstream>

int main() {
    try {
        // Указываем хост и порт
        Poco::Net::HTTPClientSession session("youtube.com", 8443);

        // Формируем GET-запрос
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "", Poco::Net::HTTPMessage::HTTP_1_1);

        // Отправляем запрос
        session.sendRequest(request);

        // Получаем ответ
        Poco::Net::HTTPResponse response;
        std::istream& responseStream = session.receiveResponse(response);

        // Выводим статус ответа
        std::cout << "HTTP Status: " << response.getStatus() << "/" << response.getReason() << std::endl;

        // Читаем тело ответа
        std::ostringstream responseBody;
        Poco::StreamCopier::copyStream(responseStream, responseBody);

        // Выводим тело ответа
        std::cout << "Response Body: " << responseBody.str() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Standard Exception: " << ex.what() << std::endl;
    } catch (const Poco::Exception& ex) {
        std::cerr << "Poco Exception: " << ex.displayText() << std::endl;
    }

    return 0;
}