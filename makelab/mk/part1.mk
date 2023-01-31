include $(HOME)/mk/config.mk

$(OUTPUT): main.o A.a.o some.a.o B.b.o 
	$(CXX) -o $@ $^
