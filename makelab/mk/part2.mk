include $(HOME)/mk/config.mk

$(OUTPUT): main.o libB.a libA.a
	$(CXX) -o $@ $^

libA.a: A.a.o notA.a.o some.a.o
	$(AR) -r $@ $^

libB.a: B.b.o
	$(AR) -r $@ $^
