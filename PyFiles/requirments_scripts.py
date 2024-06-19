import subprocess

packages = [
    'torch',
    'numpy',
    'torchvision',
    'Pillow'
]

with open('requirements.txt', 'w') as f:
    for package in packages:
        result = subprocess.run(['pip', 'show', package], stdout=subprocess.PIPE, text=True)
        for line in result.stdout.split('\n'):
            if line.startswith('Version:'):
                version = line.split()[1]
                f.write(f'{package}=={version}\n')
                break

    print("requirements.txt has been created.")