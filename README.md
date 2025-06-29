## Quickstart

To run this http server locally

1. **Prerequisites:** Ensure you have [gcc](https://gcc.gnu.org/) and C installed.
2. **Run the http server:** Execute the following command in your terminal:

   
***Clone the repository***

   ```bash
   git clone https://github.com/kunstewi/c-http-server
   ```

***Go into the directory fo c-http-server***

   ```bash
   cd c-http-server
   ```

***Run make to create the binary of main.c***

   ```bash
   make
   ```

***Execute the binary***   

   ```bash
   ./http_server
   ```

You will see "listening on port 8080" on the terminal.

go to port 8080 to see the html page that is rendered via the http server from the www folder index.html.

if you want to delete the binary run -

   ```bash
   make clean
   ```
