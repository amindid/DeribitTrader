all:
	docker build -t deribittrader . && docker run -it deribittrader