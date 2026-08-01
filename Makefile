BUILD_DIR := build
TARGET    := alisa-v-zadzerkalli_core

# Cap parallel compile jobs. An uncapped -j spawns one heavy C++ process per
# core at once and can exhaust RAM. Override on the command line: make JOBS=4
JOBS ?= 2

.PHONY: all deps run clean rebuild

all: deps
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) -j$(JOBS)

# Install the DPP Discord library from the AUR if it is not already present.
# MAKEFLAGS caps makepkg's own compile so the install cannot OOM the machine.
deps:
	@if pkg-config --exists dpp 2>/dev/null || [ -f /usr/include/dpp/dpp.h ]; then \
		echo "dpp already installed"; \
	elif command -v yay >/dev/null 2>&1; then \
		echo "dpp not found, installing from AUR with yay (JOBS=$(JOBS))"; \
		MAKEFLAGS="-j$(JOBS)" yay -S --needed --noconfirm dpp; \
	elif command -v paru >/dev/null 2>&1; then \
		echo "dpp not found, installing from AUR with paru (JOBS=$(JOBS))"; \
		MAKEFLAGS="-j$(JOBS)" paru -S --needed --noconfirm dpp; \
	else \
		echo "dpp not found and no AUR helper (yay/paru). Install dpp manually."; \
		exit 1; \
	fi
	@if pkg-config --exists cpr 2>/dev/null || [ -f /usr/include/cpr/cpr.h ]; then \
		echo "cpr already installed"; \
	elif command -v yay >/dev/null 2>&1; then \
		echo "cpr not found, installing with yay (JOBS=$(JOBS))"; \
		MAKEFLAGS="-j$(JOBS)" yay -S --needed --noconfirm cpr; \
	elif command -v paru >/dev/null 2>&1; then \
		echo "cpr not found, installing with paru (JOBS=$(JOBS))"; \
		MAKEFLAGS="-j$(JOBS)" paru -S --needed --noconfirm cpr; \
	else \
		echo "cpr not found and no AUR helper (yay/paru). Install cpr manually."; \
		exit 1; \
	fi
	@if pkg-config --exists lexbor 2>/dev/null || [ -f /usr/include/lexbor/html/html.h ]; then \
		echo "lexbor already installed"; \
	elif command -v yay >/dev/null 2>&1; then \
		echo "lexbor not found, installing from AUR with yay (JOBS=$(JOBS))"; \
		MAKEFLAGS="-j$(JOBS)" yay -S --needed --noconfirm lexbor; \
	elif command -v paru >/dev/null 2>&1; then \
		echo "lexbor not found, installing from AUR with paru (JOBS=$(JOBS))"; \
		MAKEFLAGS="-j$(JOBS)" paru -S --needed --noconfirm lexbor; \
	else \
		echo "lexbor not found and no AUR helper (yay/paru). Install lexbor manually."; \
		exit 1; \
	fi

# Loads .env into the environment so std::getenv sees DISCORD_TOKEN, then runs.
run: all
	@set -a; [ -f .env ] && . ./.env; set +a; ./$(BUILD_DIR)/$(TARGET)

clean:
	$(RM) -r $(BUILD_DIR)

rebuild: clean all
