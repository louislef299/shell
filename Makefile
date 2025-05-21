.DEFAULT_GOAL := default

default:
	@echo "did you mean to run 'zig build'?"

container:
	docker build -t lsh .

clean:
	rm -rf src/*.o lsh lsh.* .zig-cache zig-out
	docker container prune -f && docker image prune -f