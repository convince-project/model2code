# Build documentation

To build the documentation page, `plantuml` needs to be installed to generate the interactive overview picture.
Please download version `v1.2024.4` and move it to the correct location:

```bash
wget https://github.com/plantuml/plantuml/releases/download/v1.2024.4/plantuml-bsd-1.2024.4.jar
mkdir -p /usr/share/plantuml
sudo mv plantuml-bsd-1.2024.4.jar /usr/share/plantuml/plantuml.jar 
```

Then the following command needs to be run to build the documentation:

```bash
cd <path-to-convince_toolchain>/docs
pip install -r requirements.txt
make html
```

It has been tested with Python 3.8.10 and pip version 24.0.