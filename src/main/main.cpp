#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <chrono>
#include <art.h>
#include <unordered_map>
#include <queue>
#include "string_utils.h"
#include "collection.h"
#include "collection_manager.h"

using namespace std;

int main(int argc, char* argv[]) {
    const std::string state_dir_path = "/tmp/typesense-data";

    CollectionManager & collectionManager = CollectionManager::get_instance();
    collectionManager.init(store);

    Collection *collection = collectionManager.get_collection("collection");
    if(collection == nullptr) {
        collection = collectionManager.create_collection("collection", fields_to_index, rank_fields);
        std::ifstream infile(std::string(ROOT_DIR)+"test/documents.jsonl");
        //std::ifstream infile(argv[1]);

        std::string json_line;

        while (std::getline(infile, json_line)) {
            collection->add(json_line);
        }

        infile.close();
        cout << "FINISHED INDEXING!" << endl << flush;
    }

    //collection->remove("foo");

    auto begin = std::chrono::high_resolution_clock::now();
    std::vector<std::string> search_fields = {"title"};
    collection->search("the", search_fields, 1, 100);
    long long int timeMillis = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - begin).count();
    cout << "Time taken: " << timeMillis << "us" << endl;
    return 0;
}