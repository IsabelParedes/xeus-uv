/***************************************************************************
* Copyright (c) 2024, Isabel Paredes                                       *
* Copyright (c) 2024, QuantStack                                           *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <iostream>
#include <memory>
#include <uvw.hpp>

#include "xhook.hpp"
#include "xmock_interpreter.hpp"

#include "xeus-uv/xserver_uv.hpp"

#include "xeus-zmq/xzmq_context.hpp"

#include "xeus/xhistory_manager.hpp"
#include "xeus/xkernel.hpp"
#include "xeus/xkernel_configuration.hpp"


int main(int argc, char* argv[])
{
    std::cout << "[TEST] Create loop" << std::endl;
    std::shared_ptr<uvw::loop> loop_ptr = uvw::loop::get_default();

    std::cout << "[TEST] Create hook" << std::endl;
    std::unique_ptr<xeus::xhook> hook_ptr = std::make_unique<xeus::xhook>(42);

    std::cout << "[TEST] Load configuration" << std::endl;
    std::string file_name = (argc == 1) ? "connection.json" : argv[2];
    xeus::xconfiguration config = xeus::load_configuration(file_name);

    std::cout << "[TEST] Create mock interpreter" << std::endl;
    using interpreter_ptr = std::unique_ptr<xeus::xmock_interpreter>;
    interpreter_ptr interpreter = std::make_unique<xeus::xmock_interpreter>();

    auto make_xserver_lambda = [&](xeus::xcontext& context,
                                   const xeus::xconfiguration& config,
                                   nl::json::error_handler_t eh)
    {
        return xeus::make_xserver_uv(context, config, eh, loop_ptr, std::move(hook_ptr));
    };

    std::cout << "[TEST] Create kernel" << std::endl;
    xeus::xkernel kernel(config,
                         xeus::get_user_name(),
                         xeus::make_zmq_context(),
                         std::move(interpreter),
                         make_xserver_lambda,
                         xeus::make_in_memory_history_manager(),
                         nullptr); // logger

    std::cout << "[TEST] Start kernel" << std::endl;
    kernel.start();

    return 0;
}
