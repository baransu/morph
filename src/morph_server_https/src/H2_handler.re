open Morph;

let error_handler = (_client_address, ~request as _=?, _error, start_response) => {
  open H2;
  let response_body = start_response(Headers.empty);
  Body.close_writer(response_body);
};

let map_or = (~default: 'b, fn: 'a => 'b, opt: option('a)) =>
  switch (opt) {
  | Some(value) => fn(value)
  | None => default
  };

let make:
  (Opium_core.Service.t(Request.t, Response.t), Unix.sockaddr, H2.Reqd.t) =>
  unit =
  (handler, _client_address, request_descriptor) => {
    Lwt.async(() => {
      let H2.{Request.target, meth, headers, scheme: _} =
        H2.Reqd.request(request_descriptor);

      let content_length =
        H2.Headers.get(headers, "content-length")
        |> map_or(~default=128, int_of_string);

      let read_body = () =>
        Morph_server.Body.read(
          ~content_length,
          ~get_request_body=H2.Reqd.request_body,
          ~schedule_read=H2.Body.schedule_read,
          request_descriptor,
        );

      let create_response = (~headers, status) =>
        H2.Response.create(~headers, status);

      let request =
        Request.{
          target,
          meth,
          headers: H2.Headers.to_list(headers),
          read_body,
          context: Hmap.empty,
        };

      handler(request)
      |> Lwt.map((response: Response.t) => {
           let headers = H2.Headers.of_list(response.headers);
           Morph_server.Respond.respond(
             ~http_response=create_response(~headers, response.status),
             ~respond_with_string=H2.Reqd.respond_with_string,
             ~respond_with_streaming=
               H2.Reqd.respond_with_streaming(
                 ~flush_headers_immediately=true,
               ),
             ~write_string=H2.Body.write_string(~off=?None, ~len=?None),
             ~write_char=H2.Body.write_char,
             ~close_writer=H2.Body.close_writer,
             ~flush_body=H2.Body.flush,
             request_descriptor,
             response,
           );
         });
    });
  };
