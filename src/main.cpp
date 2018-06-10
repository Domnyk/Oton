#include "controller/RootController.hpp"

int main(int argc, char* argv[]) {
    RootController root_controller(argc, argv);

    return root_controller.exec();
}
