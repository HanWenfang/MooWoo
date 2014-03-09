

include common.mk

default: MooWoo

MooWoo: $(obj-MooWoo)
	$(CPP) $^ -o $@ $(LIBRARY) $(SYSLIBS)

%.o: %.cpp
	$(CPP) -Wall $(INCLUDE) -c $< -o $@

clean:
	rm -rf MooWoo $(obj-MooWoo)

