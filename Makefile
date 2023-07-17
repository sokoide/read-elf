TARGET := read-elf
SRCS := main.c
ELF := $(HOME)/tmp/riscv1/riscv1

$(TARGET): $(SRCS)
	clang -o $@ main.c -I/opt/homebrew/include -L/opt/homebrew/lib -lelf

run: $(TARGET)
	./$(TARGET) $(ELF)

clean:
	rm -rf $(TARGET) *.o
