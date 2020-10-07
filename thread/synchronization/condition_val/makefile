ALL:pc 

pc:producer_consumer

producer_consumer:producer_consumer.c
	gcc $< -o $@ -lpthread


clean:
	-rm -rf producer_consumer

.PHONY:pc clean
