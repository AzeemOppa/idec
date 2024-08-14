DEMO_ARRAY :=\
  build/$(BUILD_MODE)/demo/array/int_array_0_sorted.txt\
  build/$(BUILD_MODE)/demo/array/int_array_0_curly.txt\
  build/$(BUILD_MODE)/demo/array/jagged4_0_curly.txt\
  build/$(BUILD_MODE)/demo/array/jagged4_1_curly.txt\
  build/$(BUILD_MODE)/demo/array/cartesian_product_3_2_3.txt\
  build/$(BUILD_MODE)/demo/array/combination_6_3.txt\

.PHONY: demo_array
demo_array: bin_array $(DEMO_ARRAY) | build/$(BUILD_MODE)/demo/array

build/$(BUILD_MODE)/demo/array: | build/$(BUILD_MODE)/demo
	mkdir -p $@

build/$(BUILD_MODE)/demo/array/int_array_0_sorted.txt:\
  build/$(BUILD_MODE)/bin/array_indexed_merge_sort$(.EXE)\
  data/array/int_array_0.txt | build/$(BUILD_MODE)/demo/array
	$< < $(word 2, $^) > $@

build/$(BUILD_MODE)/demo/array/int_array_0_curly.txt:\
  build/$(BUILD_MODE)/bin/jagged1$(.EXE)\
  data/array/int_array_0.txt | build/$(BUILD_MODE)/demo/array
	$< < $(word 2, $^) > $@

build/$(BUILD_MODE)/demo/array/jagged4_0_curly.txt:\
  build/$(BUILD_MODE)/bin/jagged4$(.EXE)\
  data/array/jagged4_0.txt | build/$(BUILD_MODE)/demo/array
	$< < $(word 2, $^) > $@

build/$(BUILD_MODE)/demo/array/jagged4_1_curly.txt:\
  build/$(BUILD_MODE)/bin/jagged4$(.EXE)\
  data/array/jagged4_1.txt | build/$(BUILD_MODE)/demo/array
	$< < $(word 2, $^) > $@

build/$(BUILD_MODE)/demo/array/cartesian_product_3_2_3.txt:\
  build/$(BUILD_MODE)/bin/int_array_cartesian_product$(.EXE)\
  | build/$(BUILD_MODE)/demo/array
	$< 3 2 3 > $@

build/$(BUILD_MODE)/demo/array/combination_6_3.txt:\
  build/$(BUILD_MODE)/bin/int_array_combination$(.EXE)\
  | build/$(BUILD_MODE)/demo/array
	$< 6 3 > $@

.PHONY: demo_array_clean
demo_array_clean:
	-$(RM) $(DEMO_ARRAY)
	-$(RM) -r build/$(BUILD_MODE)/demo/array
