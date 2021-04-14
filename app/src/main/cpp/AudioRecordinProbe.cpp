//
// Created by User on 14.04.2021.
//

#include "AudioRecordinProbe.h"
#include <oboe/Oboe.h>
#include <jni.h>
#include <string>
#include <fstream>
namespace little_endian_io
{
    template <typename Word>
    std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
    {
        for (; size; --size, value >>= 8)
            outs.put( static_cast <char> (value & 0xFF) );
        return outs;
    }
}
using namespace little_endian_io;
class AudioRecordinProbe: public oboe::AudioStreamCallback {
private:
    oboe::AudioStream *stream{};
    static AudioRecordinProbe *singleton;
    explicit AudioRecordinProbe() = default;
    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override {
    }
public:
    static AudioRecordinProbe *get() {
        if (singleton == nullptr)
            singleton = new AudioRecordinProbe();
        return singleton;
    }

    bool isRecording = true;

    void StopAudioRecorder() {
       this->isRecording = false;
    }
    void StartRecordingOpenSLES (const char* fPath) {
        this->isRecording = true;
        oboe::AudioStreamBuilder builder;
        builder.setDirection(oboe::Direction::Input);
        builder.setPerformanceMode(oboe::PerformanceMode::None);
        builder.setFormat(oboe::AudioFormat::I16);
        builder.setChannelCount(oboe::ChannelCount::Mono);
        builder.setInputPreset(oboe::InputPreset::Unprocessed);
        builder.setSharingMode(oboe::SharingMode::Shared);
        builder.setSampleRate(48000);
        builder.setAudioApi(oboe::AudioApi::OpenSLES);
        // можно, и нужно наверное в реальном проекте использовать libsndfile но мы просто запишем заголовок
        // кодом который уже много лет гуляет по интернету, например здесь -
        // https://www.cplusplus.com/forum/beginner/166954/
        std::ofstream f;
        //const char *path = "/storage/emulated/0/Music/record.wav";
        const char *path = fPath;
        f.open(path, std::ios::binary);
        // Write the file headers
        f << "RIFF----WAVEfmt ";     // (chunk size to be filled in later)
        write_word(f, 16, 4);  // no extension data
        write_word(f, 1, 2);  // PCM - integer samples
        write_word(f, 1, 2);  // one channel (mono) or two channels (stereo file)
        write_word(f, 48000, 4);  // samples per second (Hz)
        write_word(f, (48000 * 16 * 1) / 8, 4);  // (Sample Rate * BitsPerSample * Channels) / 8
        write_word(f, 4,2);  // data block size (size of two integer samples, one for each channel, in bytes)
        write_word(f, 16, 2);  // number of bits per sample (use a multiple of 8)

        // Write the data chunk header
        size_t data_chunk_start = f.tellp();
        f << "data----";  // (chunk size to be filled in later)

        // Write the audio samples
        constexpr double two_pi = 6.283185307179586476925286766559;
        constexpr double max_amplitude = 32760;  // "volume"

        oboe::Result result = builder.openStream(&stream);
        if (result != oboe::Result::OK) {
            return;
        }

        result = stream->requestStart();
        if (result != oboe::Result::OK) {
            return;
        }
        oboe::StreamState currstreamstate = stream->getState();
        if (currstreamstate == oboe::StreamState::Started) {

            constexpr int kMillisecondsToRecord = 2;
            auto requestedFrames = (int32_t) (kMillisecondsToRecord *
                                              (stream->getSampleRate() / oboe::kMillisPerSecond));
            int16_t mybuffer[requestedFrames];
            constexpr int64_t kTimeoutValue = 3 * oboe::kNanosPerMillisecond;

            int framesRead = 0;
            do {
                auto result = stream->read(mybuffer, requestedFrames, 0);
                if (result != oboe::Result::OK) {
                    break;
                }
                framesRead = result.value();
                if (framesRead > 0) {
                    break;
                }
            } while (framesRead != 0);

            while (isRecording) {
                auto result = stream->read(mybuffer, requestedFrames, kTimeoutValue * 1000);
                if (result == oboe::Result::OK) {
                    auto nbFramesRead = result.value();

                    for (int i = 0; i < nbFramesRead; i++) {
                        write_word(f, (int) (mybuffer[i]), 2);
                    }
                } else {
                    auto error = convertToText(result.error());
                }
            }
        } //if
        stream->requestStop();
        stream->close();

        // (We'll need the final file size to fix the chunk sizes above)
        size_t file_length = f.tellp();

        // Fix the data chunk header to contain the data size
        f.seekp( data_chunk_start + 4 );
        write_word( f, file_length - data_chunk_start + 8 );

        // Fix the file header to contain the proper RIFF chunk size, which is (file size - 8) bytes
        f.seekp( 0 + 4 );
        write_word( f, file_length - 8, 4 );
        f.close();


    }
};
AudioRecordinProbe *AudioRecordinProbe::singleton = nullptr;