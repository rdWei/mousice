
<p align="center">
    <img src="images/logo.png">
</p>

<h6 align="center">
Suckless CLI YouTube-Music Player
    </h6>

<h1></h1>

<br>

<div style="display: inline;" align="center">
    <a href="">
        Support Me
    </a>
</div>

<br>

<p align="center">
    <img src="images/screen.png">
</p>

### Table of Contents

- [Dependencies](#dependencies)
- [Compile](#compile)
- [Usage](#usage)
- [TODO](#todo)
- [Contributing](#contributing)

### Dependencies

To run this application, you need to install the following dependencies:

- `mpv` - A versatile media player.
- `chafa` (version 1.14.0 or later) - A command-line tool for converting images to ASCII/ANSI art.
- `yt-dlp` - A YouTube video downloader with additional features.
- `curl` - A command-line tool for transferring data with URLs.

### Compile

```bash
mkdir build
cd build
cmake ..
make
./mousice bohemian_rhapsody
```

### Usage
`./mousice bohemian_rhapsody`

### TODO
- [ ] Playlist support
- [ ] Close program when the song end
- [ ] Change [invidius](https://docs.invidious.io/instances/) server if one is offline
### Contributing
If you'd like to contribute, you can submit a [pull request](https://github.com/rdWei/mousice/pulls) with your changes or open an [issue](https://github.com/rdWei/mousice/pulls) to report any problems or feature requests.
