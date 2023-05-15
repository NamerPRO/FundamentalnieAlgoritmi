#include <iostream>
#include <string>
#include <utility>

#include "./pipeline_manager/pipeline.h"



int main() {
    npipeline::pipeline pipe(npipeline::pipeline::tree_type::avl);
    pipe.run("./insts");

    // npipeline::pipeline::collection * my_collection = a.insert_collection(
    //     "MAIN_POOL",
    //     "SCHEME_1",
    //     "super cool collection"
    // );

    // npipeline::pipeline::pipeline_passage db = {
    //         1,
    //         2,
    //         {
    //             111,
    //             "222",
    //             "333"
    //         },
    //         "aaa",
    //         "bbb",
    //         "ccc",
    //         "ddd",
    //         "eee",
    //         "fff"
    // };

    // my_collection->insert({ 1, 2 }, std::move(&db));

    // std::cout << my_collection->find({1, 2})->commit_information.developer_email << std::endl;

    // npipeline::pipeline::pipeline_passage db2 = {
    //         1,
    //         2,
    //         {
    //             111,
    //             "222",
    //             "whats the kursach?"
    //         },
    //         "aaa",
    //         "bbb",
    //         "ccc",
    //         "ddd",
    //         "eee",
    //         "fff"
    // };

    // my_collection->insert({ 1, 2 }, std::move(&db2));

    // std::cout << my_collection->find({1, 2})->commit_information.developer_email << std::endl;

    // a.remove_pool("MAIN_POOL");

    // std::cout << a.find("MAIN_POOL") << std::endl;

    return 0;
}