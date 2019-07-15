package kvs

type entry struct {
	key   string
	value string
}

type query struct {
	key    string
	respCh chan<- response
}

type response struct {
	value string
	ok    bool
}

type Database struct {
	storeCh chan entry
	queryCh chan query
	closeCh chan bool
}

func New() *Database {
	d := &Database{
		storeCh: make(chan entry),
		queryCh: make(chan query),
		closeCh: make(chan bool),
	}

	go func() {
		db := map[string]string{}

		for {
			select {
			case kv := <-d.storeCh:
				db[kv.key] = kv.value

			case q := <-d.queryCh:
				v, ok := db[q.key]
				q.respCh <- response{value: v, ok: ok}

			case <-d.closeCh:
				return
			}
		}
	}()

	return d
}

func (d *Database) Close() error {
	d.closeCh <- true
	return nil
}

func (d *Database) Store(k, v string) {
	d.storeCh <- entry{key: k, value: v}
}

func (d *Database) Fetch(k string) (string, bool) {
	ch := make(chan response)
	d.queryCh <- query{key: k, respCh: ch}
	resp := <-ch
	return resp.value, resp.ok
}
