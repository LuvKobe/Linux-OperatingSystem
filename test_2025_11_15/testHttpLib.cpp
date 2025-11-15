#include "httplib.h"

const std::string root_path = "./wwwroot";

int main()
{
    httplib::Server svr;

    svr.set_base_dir(root_path.c_str()); // 引入wwwroot目录
    svr.Get("/hi", [](const httplib::Request &req, httplib::Response &rsp)
            { rsp.set_content("Hello 荣大！！!", "text/plain; charset=utf-8"); });

    svr.listen("0.0.0.0", 8081);
    return 0;
}