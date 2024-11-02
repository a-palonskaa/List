ARR_LIST_DIR  = two_arrays_list
PTR_LIST_DIR  = list_on_pointers
INTR_LIST_DIR = intrusive_list
COMMON_DIR = common

LIBS_DIR = $(COMMON_DIR)
SUBDIRS = $(COMMON_DIR) $(ARR_LIST_DIR) $(PTR_LIST_DIR) $(INTR_LIST_DIR)

all: meow

meow:
	@for dir in $(ARR_LIST_DIR); do \
		$(MAKE) -C $$dir all;       \
	done

myrr:
	@for dir in $(PTR_LIST_DIR); do \
		$(MAKE) -C $$dir all;       \
	done

kitty_kitty:
	@for dir in $(INTR_LIST_DIR); do \
		$(MAKE) -C $$dir all;        \
	done

lib:
	@for dir in $(COMMON_DIR); do \
		$(MAKE) -C $$dir all;     \
	done

clean:
	@for dir in $(SUBDIRS); do  \
		$(MAKE) -C $$dir clean; \
	done
