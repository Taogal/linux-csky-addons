/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <sound/soc.h>

static const struct snd_soc_dapm_widget dummy_codec_dapm_widgets[] = {
	SND_SOC_DAPM_OUTPUT("VOUTL"),
	SND_SOC_DAPM_OUTPUT("VOUTR"),
};

static const struct snd_soc_dapm_route dummy_codec_dapm_routes[] = {
	{ "VOUTL", NULL, "Playback" },
	{ "VOUTR", NULL, "Playback" },
};

#define STUB_RATES	SNDRV_PCM_RATE_8000_192000
#define STUB_FORMATS	(SNDRV_PCM_FMTBIT_S8 | \
			 SNDRV_PCM_FMTBIT_U8 | \
			 SNDRV_PCM_FMTBIT_S16_LE | \
			 SNDRV_PCM_FMTBIT_U16_LE | \
			 SNDRV_PCM_FMTBIT_S24_LE | \
			 SNDRV_PCM_FMTBIT_U24_LE | \
			 SNDRV_PCM_FMTBIT_S32_LE | \
			 SNDRV_PCM_FMTBIT_U32_LE)

static struct snd_soc_dai_driver dummy_codec_dai = {
	.name = "csky-dummy-codec-dai",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = STUB_RATES,
		.formats = STUB_FORMATS,
	},
};

static const struct snd_soc_codec_driver soc_codec_dev_dummy_codec = {
	.component_driver = {
		.dapm_widgets		= dummy_codec_dapm_widgets,
		.num_dapm_widgets	= ARRAY_SIZE(dummy_codec_dapm_widgets),
		.dapm_routes		= dummy_codec_dapm_routes,
		.num_dapm_routes	= ARRAY_SIZE(dummy_codec_dapm_routes),
	},
};

static int dummy_codec_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev,
				      &soc_codec_dev_dummy_codec,
				      &dummy_codec_dai, 1);
}

static int dummy_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static const struct of_device_id dummy_codec_codec_match[] = {
	{ .compatible = "csky,i2s-dummy-codec", },
	{}
};
MODULE_DEVICE_TABLE(of, dummy_codec_codec_match);

static struct platform_driver dummy_codec_codec_driver = {
	.probe	= dummy_codec_probe,
	.remove	= dummy_codec_remove,
	.driver	= {
		.name		= "dummy-codec",
		.of_match_table	= dummy_codec_codec_match,
	},
};
module_platform_driver(dummy_codec_codec_driver);

MODULE_DESCRIPTION("C-SKY dummy codec driver");
MODULE_LICENSE("GPL");

