#ifndef PIPELINE
#define PIPELINE

#include <any>
#include <functional>
#include <map>
#include <variant>
#include <vector>
#include <fstream>
#include <chrono>

#include "./pipeline_base.h"
#include "../interpritator_manager/interpritator.h"

#include "../nsplay/splay_tree.h"
#include "../nbtree/btree.h"
#include "../navl/avl_tree.h"
#include "./standard_pipeline_interpriter_commands.h"
#include "./pipeline_interpriters.h"

namespace npipeline {

    class pipeline : private ntools::nmalloc, private pipeline_base {

    private:

        data_base * _dbase;

        data_base_with_developer_login_key * _dbase_with_developer_login_key;

        ninterpritator::interpritator * _interpriter;

        invoker * _invoker;

        nmemory::memory * _allocator;

    public:

        void run() {
            _interpriter->interpritate();
        }

        pipeline(
            tree_type container_type,
            interpriter_type interpriter_to_use,
            std::string && path_to_file = "",
            nmemory::memory * allocator = nullptr
        ) : _allocator(allocator), ntools::nmalloc(allocator) {
            switch (container_type) {
            
            case tree_type::avl:
                _dbase = new navl::avl_tree<std::string, pool, pipeline_comporators::standard_string_comporator>(_allocator);
                _dbase_with_developer_login_key = new navl::avl_tree<std::string, pool_with_developer_login_key, pipeline_comporators::standard_string_comporator>(_allocator);
                break;
            
            case tree_type::splay:
                _dbase = new nsplay::splay_tree<std::string, pool, pipeline_comporators::standard_string_comporator>(_allocator);
                _dbase_with_developer_login_key = new nsplay::splay_tree<std::string, pool_with_developer_login_key, pipeline_comporators::standard_string_comporator>(_allocator);
                break;
            
            case tree_type::btree:
                _dbase = new nbtree::btree<std::string, pool, pipeline_comporators::standard_string_comporator>(_allocator, 2);
                _dbase_with_developer_login_key = new nbtree::btree<std::string, pool_with_developer_login_key, pipeline_comporators::standard_string_comporator>(_allocator, 2);
                break;
            
            }

            _invoker = new invoker();

            switch (interpriter_to_use) {

            case interpriter_type::file_input_interpriter:
                _interpriter = new pipeline_interpriter(_dbase, _dbase_with_developer_login_key, path_to_file, _invoker, _allocator);
                break;

            case interpriter_type::user_input_interpriter:
                _interpriter = new user_interpriter(_dbase, _dbase_with_developer_login_key, _invoker, _allocator);
                break;
            
            }
        }

        ~pipeline() {
            delete _interpriter;
            delete _dbase;
            delete _dbase_with_developer_login_key;
            delete _invoker;
        }

    };

}

#endif /* PIPELINE */
