# Frank Reader
Application that allows user to load books from its library, read them using the built-in translator and save unknown phrases to a personal dictionary for later study. It makes easier to read in English for Russian-speaking people.

Created during the spring semester of the first year of the Applied Mathematics and Informatics program at the Higher School of Economics of St. Petersburg.

## Dependencies for Ubuntu 20.04:
- Qt5 (`qt5-default`)
- `mysql-server`
- `libmysqlcppconn-dev`
- `nlohmann-json-dev`
- `libcrypto++-dev`
- `libssl-dev`
- `libcurl-dev`

Also it is nesessary to create files `config.json` with your own information. You can find examples of these files in `client` and `server` directories.

See detailed description of application architecture [here](https://drive.google.com/file/d/11VDtDVa_EF68MFzfnXAaD7f5yl2ykQMO/view?usp=sharing).
