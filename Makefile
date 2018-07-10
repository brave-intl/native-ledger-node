.PHONY: build
.PHONY: test
.PHONY: clean

build:
	 ./node_modules/.bin/node-gyp configure && ./node_modules/.bin/node-gyp build
dev:
	 ./node_modules/.bin/node-gyp configure && ./node_modules/.bin/node-gyp -j 16 build --debug
test:
	./node_modules/node-gyp/gyp/gyp_main.py --generator-output=./build --depth=. -f ninja binding.gyp
	./node_modules/node-gyp/gyp/gyp_main.py --generator-output=./build --depth=. -f xcode binding.gyp
	ninja -C -I build/out/Default -f build.ninja
	./build/out/Default/test || [ $$? -eq 0 ]

clean:
	rm -Rf build
