# VSTi
Audio delay effect

The provided code implements an audio delay effect using the PortAudio library. It defines a `Delay` class that handles the storage and processing of the delay effect. A circular buffer is used to store audio samples and calculate the delay. The `main` function initializes PortAudio, sets up, and runs an audio stream that applies the delay to incoming samples. The audio stream is managed through the `audioCallback` function, which reads input samples, applies the delay, and outputs the result to the output buffer. If any errors occur during the process, the system prints the error and terminates execution appropriately.
