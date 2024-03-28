#include <pthread.h>
#include <crow.h>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

// g++ SimpleFileDownloadServer.cpp -o sfds -lpthread -DCROW_ENABLE_SSL -lssl -lcrypto

using namespace std;

vector<string> files(){

    const std::string path = "FilesToUpload/";
    vector<string> fileNames;
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
        ifstream archivo(entry.path());
        if (archivo.is_open()){
            fileNames.push_back(entry.path().filename().string());
        }
    }
    }
    return fileNames;
}

int listener(){

    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([](){auto page = crow::mustache::load_text("index.html");return page;});
    CROW_ROUTE(app, "/requestjson.js")([]{
    crow::mustache::context ctx;
    return crow::mustache::load_text("requestjson.js");
    });

    CROW_ROUTE(app, "/FileNames").methods("GET"_method)
    ([](const crow::request& req) {
        crow::json::wvalue response;
        vector<string> fl = files();
        response = fl;
        return crow::response(response);
    });

    CROW_ROUTE(app, "/download").methods("POST"_method)([](const crow::request &req, crow::response &res) {

        auto x = crow::json::load(req.body);

        if (!x) {
            res.set_header("Content-Type", "text/plain");
            res.body = "Bad Format";
            return res.end();
        }
        // Abrir el archivo que se va a descargar

        vector<string>AvailableFile = files();
        string QueryFront =  x["resourse"].s();

        for(int i = 0 ; i < AvailableFile.size() ; i++ ){
            if(QueryFront == AvailableFile[i]){

                std::ifstream file("FilesToUpload/"+AvailableFile[i] , std::ios::in | std::ios::binary);
                if (file) {
                    // Set headers for the HTTP response
                    res.set_header("Content-Type", "application/octet-stream");
                    res.set_header("Content-Disposition", "attachment; filename=" + AvailableFile[i]);

                    // Stream the file to the response
                    constexpr std::streamsize chunkSize = 4096;
                    char buffer[chunkSize];
                    std::streamsize bytesRead;
                    while ((bytesRead = file.read(buffer, chunkSize).gcount()) > 0) {
                        std::string bufferStr(buffer, bytesRead);
                        res.write(bufferStr);
                    }
                    file.close();
                    res.end();
                } 
            }
        }
        res.code = 404;
        res.write("Error al leer el archivo");
        res.end();

    });

    app.port(12012).run();
    

    return 0;

}

int main(){
listener(); 
 
return 0;

}