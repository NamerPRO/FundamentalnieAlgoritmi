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
#include "./pipeline_data_save.h"

#include "../pipeline_commands_manager/command_invoker.h"

#include "../nsplay/splay_tree.h"
#include "../nbtree/btree.h"

namespace npipeline {

    class pipeline : private ntools::nmalloc, private pipeline_base {

    private:

        data_base * _dbase;

        data_base_with_developer_login_key * _dbase_with_developer_login_key;

        nmemory::memory * _allocator;

        npipeline_tools::invoker * _invoker;

    public:

        data_base * get_dbase() {
            return _dbase;
        }

        data_base_with_developer_login_key * get_dbase_with_developer_login_key() {
            return _dbase_with_developer_login_key;
        }

        nmemory::memory * get_allocator() {
            return _allocator;
        }

        npipeline_tools::invoker * get_invoker() {
            return _invoker;
        }

        tree_type _main_tree_type;

    public:

        pipeline(
            tree_type container_type,
            nmemory::memory * allocator = nullptr
        ) : _allocator(allocator), ntools::nmalloc(allocator), _main_tree_type(container_type) {
            
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

            _invoker = new npipeline_tools::invoker();
        }

        ~pipeline() {
            delete _allocator;
            delete _dbase;
            delete _dbase_with_developer_login_key;
            delete _invoker;
        }

    };

}

#endif /* PIPELINE */
