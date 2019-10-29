type t = {
  target: string,
  meth: Method.t,
  headers: Headers.t,
  read_body: unit => Lwt.t(string),
  context: Hmap.t,
};

let make =
    (
      ~meth=`GET,
      ~headers=Headers.empty,
      ~read_body=() => Lwt.return(""),
      ~context=Hmap.empty,
      target,
    ) => {
  target,
  meth,
  headers,
  read_body,
  context,
};
