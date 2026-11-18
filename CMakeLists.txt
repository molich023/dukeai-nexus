
cmake_minimum_required(VERSION 3.16)

project(DukeAINexus VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# JSON dependency
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED IMPORTED_TARGET jsoncpp)

# Core library
add_library(dukeai_energy
    src/ebm/energy_ranker.cpp
    src/text/tokenizer.cpp
    src/text/feature_extractor.cpp
    src/retrieval/retriever.cpp
    src/documents/document_loader.cpp
    src/documents/document_ingestion.cpp
    src/metadata/document_metadata.cpp
    src/metadata/document_metadata_loader.cpp
)

target_include_directories(dukeai_energy
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(dukeai_energy
    PRIVATE -Wall -Wextra -Wpedantic
)

target_link_libraries(dukeai_energy
    PRIVATE PkgConfig::JSONCPP
)

# Main executable
add_executable(dukeai_ranker src/ebm/main.cpp)
target_link_libraries(dukeai_ranker PRIVATE dukeai_energy)

# Test executables
add_executable(dukeai_text_tests tests/test_text.cpp)
target_link_libraries(dukeai_text_tests PRIVATE dukeai_energy)

add_executable(dukeai_retrieval_tests tests/test_retrieval.cpp)
target_link_libraries(dukeai_retrieval_tests PRIVATE dukeai_energy)

add_executable(dukeai_document_loader_tests
    tests/test_document_loader.cpp
)
target_link_libraries(dukeai_document_loader_tests PRIVATE dukeai_energy)

add_executable(dukeai_document_ingestion_tests
    tests/test_document_ingestion.cpp
)
target_link_libraries(
    dukeai_document_ingestion_tests PRIVATE dukeai_energy
)

add_executable(dukeai_document_metadata_tests
    tests/test_document_metadata.cpp
)
target_link_libraries(dukeai_document_metadata_tests PRIVATE dukeai_energy)

# Register tests
enable_testing()

add_test(
    NAME ranker_smoke_test
    COMMAND dukeai_ranker
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/data/ranking_pairs.tsv
)

add_executable(dukeai_document_metadata_loader_tests
    tests/test_document_metadata_loader.cpp
)

target_link_libraries(
    dukeai_document_metadata_loader_tests
    PRIVATE dukeai_energy
)
add_test(NAME text_processing_test COMMAND dukeai_text_tests)
add_test(NAME retrieval_test COMMAND dukeai_retrieval_tests)
add_test(NAME document_loader_test COMMAND dukeai_document_loader_tests)
add_test(NAME document_metadata_test COMMAND dukeai_document_metadata_tests)
add_test(
    NAME document_ingestion_test
    COMMAND dukeai_document_ingestion_tests
)

add_test(
    NAME document_metadata_loader_test
    COMMAND dukeai_document_metadata_loader_tests
            ${CMAKE_CURRENT_SOURCE_DIR}/data/metadata/theory_of_computer_science.json
)

# Standalone document ingestion and retrieval demonstration
add_executable(dukeai_demo src/demo/main.cpp)
target_link_libraries(dukeai_demo PRIVATE dukeai_energy)

add_test(
    NAME retrieval_demo_test
    COMMAND dukeai_demo ${CMAKE_CURRENT_SOURCE_DIR}/data/demo
)
