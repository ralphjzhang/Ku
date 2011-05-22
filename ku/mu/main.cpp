#include "level.hpp"
#include "sink/console.hpp"
#include "sink/simple_file.hpp"
#include "reader/formatted_reader.hpp"
#include "writer/formatted_writer.hpp"
#include "macro.hpp"
#include "mu.hpp"


void simple_test1();
void formatted_test1();
//void threaded_writer_test1();


using namespace ku::mu;

int main()
{
    simple_test1();
    formatted_test1();
    //threaded_writer_test1();

    std::cout << "Exiting main thread..." << std::endl;

    return 0;
}
