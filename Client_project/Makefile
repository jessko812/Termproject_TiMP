CXX      := g++
CXXFLAGS := -std=c++17 -Wall -O2
LIBS     := -lUnitTest++

# ──────────────────────────────────────────────────────────────
# Основная программа
client: main.cpp interface.cpp file_handler.cpp network_manager.cpp linux_authenticator.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# ──────────────────────────────────────────────────────────────
# Модульные тесты
test_interface: test_interface.cpp interface.cpp
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

test_file_handler: test_file_handler.cpp file_handler.cpp
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

test_linux_authenticator: test_linux_authenticator.cpp linux_authenticator.cpp
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

test_network_manager: test_network_manager.cpp linux_authenticator.cpp
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

# ──────────────────────────────────────────────────────────────
# Удобные цели
all: client

tests: test_interface test_file_handler test_linux_authenticator test_network_manager

runtests: tests
	@echo "╔═══════════════════════════════════════════════════╗"
	@echo "║               ЗАПУСК ВСЕХ ТЕСТОВ                  ║"
	@echo "╚═══════════════════════════════════════════════════╝"
	@echo "=== Interface тест ==="
	@-./test_interface 2>/dev/null && echo "✓ Interface          OK" || echo "✗ Interface          FAILED или ПРОПУЩЕН"
	@echo ""
	@echo "=== FileHandler тест ==="
	@-./test_file_handler 2>/dev/null && echo "✓ FileHandler        OK" || echo "✗ FileHandler        FAILED"
	@echo ""
	@echo "=== LinuxAuthenticator тест ==="
	@-./test_linux_authenticator 2>/dev/null && echo "✓ LinuxAuthenticator OK" || echo "✗ LinuxAuthenticator FAILED"
	@echo ""
	@echo "=== NetworkManager тест ==="
	@-./test_network_manager 2>/dev/null && echo "✓ NetworkManager     OK" || echo "✗ NetworkManager     FAILED"
	@echo "═══════════════════════════════════════════════════════"
	@echo "    ТЕСТИРОВАНИЕ ЗАВЕРШЕНО"
	@echo "═══════════════════════════════════════════════════════"

clean:
	rm -f client test_interface test_file_handler test_linux_authenticator test_network_manager *.bin *.conf

.PHONY: all tests runtests clean
