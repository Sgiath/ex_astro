ARCHIVE_NAME = cspice.tar.Z
ARCHIVE_URL = https://naif.jpl.nasa.gov/pub/naif/toolkit//C/PC_Linux_GCC_64bit/packages/$(ARCHIVE_NAME)

# directories
TARGET_DIR := ./priv
SRC_DIR := ./c_src
SPICE_SRC_DIR = $(SRC_DIR)/cspice

# compilation
CC = gcc

# Erlang headers (env variable comes from :elixir_make dependency)
CFLAGS += -I$(ERTS_INCLUDE_DIR)

# General C flags
CFLAGS += -fPIC -finline-functions -Wall -Wmissing-prototypes

# C SPICE libraries
CFLAGS += -I$(SPICE_SRC_DIR)/include
LDFLAGS += -L$(SPICE_SRC_DIR)/lib -l:cspice.a -l:csupport.a

# ERFA libraries
LDFLAGS += -lerfa -lgmp

.PHONY: all
all: $(TARGET_DIR)/time.so $(TARGET_DIR)/ephemeris.so $(TARGET_DIR)/support.so

# NIFs compilation

$(TARGET_DIR)/%.so: $(SRC_DIR)/%.c $(SRC_DIR)/utils.h $(SPICE_SRC_DIR)/lib/cspice.a
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

$(SPICE_SRC_DIR)/lib/cspice.a:
	@rm -rf $(SPICE_SRC_DIR)
	echo "Downloading library files..."
	@wget $(ARCHIVE_URL)
	echo "Extracting files..."
	@gzip -d $(ARCHIVE_NAME)
	@tar xfv cspice.tar
	@mv cspice $(SRC_DIR)
	@rm cspice.tar

# cleaning

.PHONY: clean
clean:
	@rm -f $(TARGET_DIR)/*.so
	@rm -rf $(SPICE_SRC_DIR)
