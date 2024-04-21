#include <iostream>
#include <deque>
#include <cmath>
#include "portaudio.h" // Include the header for PortAudio

#define SAMPLE_RATE 44100
#define DELAY_SECONDS 0.5
#define FRAMES_PER_BUFFER 256

class Delay {
public:
    // The Delay class' constructor
    Delay(int delaySamples) : _delaySamples(delaySamples) {
        _buffer = std::deque<float>(delaySamples, 0);
    }

    // Apply delay using the process function
    float process(float input) {
        float output = _buffer.front();
        _buffer.pop_front();
        _buffer.push_back(input);
        return output;
    }

private:
    int _delaySamples;
    std::deque<float> _buffer;
};

// Audio callback function
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData);

// Main function
int main() {
    PaStream *stream;
    Delay delay((int)(DELAY_SECONDS * SAMPLE_RATE)); // Construct a Delay object with calculated delay samples.

    PaError err;

    err = Pa_Initialize(); // Initialize PortAudio
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return -1; // Return error code if initialization fails
    }

    err = Pa_OpenDefaultStream(&stream, 1, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, &delay); // Open the default stream
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate(); // Terminate PortAudio if stream opening fails
        return -1; // Return error code
    }

    err = Pa_StartStream(stream); // Start the stream
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream); // Close the stream if starting fails
        Pa_Terminate(); // Terminate PortAudio
        return -1; // Return error code
    }

    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    err = Pa_StopStream(stream); // Stop the stream
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    }
    
    err = Pa_CloseStream(stream); // Close the stream
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    }

    Pa_Terminate(); // Terminate PortAudio

    return 0; // Success returned
}

// The audio callback function's definition
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData) {
    float *out = (float*)outputBuffer;
    float *in = (float*)inputBuffer;
    Delay *delay = (Delay*)userData;

    for(unsigned long i = 0; i < framesPerBuffer; i++) {
        float input = *in++;
        float output = delay->process(input);
        *out++ = output;
    }

    return paContinue; // Go back to the pageMaintain the stream's activity going forward.
}