FROM golang:1.9
WORKDIR /go/src/github.com/snsinfu/bit4/test223-influxdb_gateway/src
COPY . .
ENV CGO_ENABLED=0
RUN go get -d ./...
RUN go build -o /gateway

FROM scratch
COPY --from=0 /gateway /gateway
CMD ["/gateway"]
