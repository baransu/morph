type t = {
  target: string,
  meth: Method.t,
  headers: Headers.t,
  read_body: unit => Lwt.t(string),
  context: Hmap.t,
};

let make:
  (
    ~meth: Method.t=?,
    ~headers: Headers.t=?,
    ~read_body: unit => Lwt.t(string)=?,
    ~context: Hmap.t=?,
    string
  ) =>
  t;
