add_unit_test(my_dependant_module "my_dependant_module;cesl_compiler")

# Adding extra target sources, might be needed when mocking out
# functions from a library.
# here's an example for situations where this would be needed:
#
#target_sources(my_dependant_module_unittest
#    PRIVATE
#        "${CMAKE_CURRENT_LIST_DIR}/../my_dependant_module.c"
#)
