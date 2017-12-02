/******************************************************************************
Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_rng.c
Version       : Initial Draft
Author        : Hisilicon
Created       : 2012/07/10
Last Modified :
Description   :	sample for hash
Function List :
History       :
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <assert.h> 
#include "hi_type.h"
#include "hi_unf_cipher.h"
#include "hi_mmz_api.h"
#include "config.h"

#define HI_ERR_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_CIPHER(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)

#ifdef CIPHER_RSA_SUPPORT
static HI_S32 printBuffer(const HI_CHAR *string, const HI_U8 *pu8Input, HI_U32 u32Length)
{
    HI_U32 i = 0;
    
    if ( NULL != string )
    {
        printf("%s\n", string);
    }

    for ( i = 0 ; i < u32Length; i++ )
    {
        if( (i % 16 == 0) && (i != 0)) printf("\n");
        printf("0x%02x ", pu8Input[i]);
    }
    printf("\n");

    return HI_SUCCESS;
}

static unsigned char test_data[] = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"};
unsigned char sha256_sum[32] =
{   
    0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39, 
    0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1,
};

static HI_U8 N[]  = 
{
    0x82, 0x78, 0xA0, 0xC5, 0x39, 0xE6, 0xF6, 0xA1, 0x5E, 0xD1, 0xC6, 0x8B, 0x9C, 0xF9, 0xC4, 0x3F, 
    0xEA, 0x19, 0x16, 0xB0, 0x96, 0x3A, 0xB0, 0x5A, 0x94, 0xED, 0x6A, 0xD3, 0x83, 0xE8, 0xA0, 0xFD, 
    0x01, 0x5E, 0x92, 0x2A, 0x7D, 0x0D, 0xF9, 0x72, 0x1E, 0x03, 0x8A, 0x68, 0x8B, 0x4D, 0x57, 0x55, 
    0xF5, 0x2F, 0x9A, 0xC9, 0x45, 0xCF, 0x9B, 0xB7, 0xF5, 0x11, 0x94, 0x7A, 0x16, 0x0B, 0xED, 0xD9, 
    0xA3, 0xF0, 0x63, 0x8A, 0xEC, 0xD3, 0x21, 0xAB, 0xCF, 0x74, 0xFC, 0x6B, 0xCE, 0x06, 0x4A, 0x51, 
    0xC9, 0x7C, 0x7C, 0xA3, 0xC4, 0x10, 0x63, 0x7B, 0x00, 0xEC, 0x2D, 0x02, 0x18, 0xD5, 0xF1, 0x8E, 
    0x19, 0x7F, 0xBE, 0xE2, 0x45, 0x5E, 0xD7, 0xA8, 0x95, 0x90, 0x88, 0xB0, 0x73, 0x35, 0x89, 0x66, 
    0x1C, 0x23, 0xB9, 0x6E, 0x88, 0xE0, 0x7A, 0x57, 0xB0, 0x55, 0x8B, 0x81, 0x9B, 0x9C, 0x34, 0x9F, 
    0x86, 0x0E, 0x15, 0x94, 0x2C, 0x6B, 0x12, 0xC3, 0xB9, 0x56, 0x60, 0x25, 0x59, 0x3E, 0x50, 0x7B, 
    0x62, 0x4A, 0xD0, 0xF0, 0xB6, 0xB1, 0x94, 0x83, 0x51, 0x66, 0x6F, 0x60, 0x4D, 0xEF, 0x8F, 0x94, 
    0xA6, 0xD1, 0xA2, 0x80, 0x06, 0x24, 0xF2, 0x6E, 0xD2, 0xC7, 0x01, 0x34, 0x8D, 0x2B, 0x6B, 0x03, 
    0xF7, 0x05, 0xA3, 0x99, 0xCC, 0xC5, 0x16, 0x75, 0x1A, 0x81, 0xC1, 0x67, 0xA0, 0x88, 0xE6, 0xE9, 
    0x00, 0xFA, 0x62, 0xAF, 0x2D, 0xA9, 0xFA, 0xC3, 0x30, 0x34, 0x98, 0x05, 0x4C, 0x1A, 0x81, 0x0C, 
    0x52, 0xCE, 0xBA, 0xD6, 0xEB, 0x9C, 0x1E, 0x76, 0x01, 0x41, 0x6C, 0x34, 0xFB, 0xC0, 0x83, 0xC5, 
    0x4E, 0xB3, 0xF2, 0x5B, 0x4F, 0x94, 0x08, 0x33, 0x87, 0x5E, 0xF8, 0x39, 0xEF, 0x7F, 0x72, 0x94, 
    0xFF, 0xD7, 0x51, 0xE8, 0xA2, 0x5E, 0x26, 0x25, 0x5F, 0xE9, 0xCC, 0x2A, 0x7D, 0xAC, 0x5B, 0x35
};
static HI_U8 E[]  = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};
static HI_U8 D[]  = 
{
    0x49, 0x7E, 0x93, 0xE9, 0xA5, 0x7D, 0x42, 0x0E, 0x92, 0xB0, 0x0E, 0x6C, 0x94, 0xC7, 0x69, 0x52, 
    0x2B, 0x97, 0x68, 0x5D, 0x9E, 0xB2, 0x7E, 0xA6, 0xF7, 0xDF, 0x69, 0x5E, 0xAE, 0x9E, 0x7B, 0x19, 
    0x2A, 0x0D, 0x50, 0xBE, 0xD8, 0x64, 0xE7, 0xCF, 0xED, 0xB2, 0x46, 0xE4, 0x2F, 0x1C, 0x29, 0x07, 
    0x45, 0xAF, 0x44, 0x3C, 0xFE, 0xB3, 0x3C, 0xDF, 0x7A, 0x10, 0x26, 0x18, 0x43, 0x95, 0x02, 0xAD, 
    0xA7, 0x98, 0x81, 0x2A, 0x3F, 0xCF, 0x8A, 0xD7, 0x12, 0x6C, 0xAE, 0xC8, 0x37, 0x6C, 0xF9, 0xAE, 
    0x6A, 0x96, 0x52, 0x4B, 0x99, 0xE5, 0x35, 0x74, 0x93, 0x87, 0x76, 0xAF, 0x08, 0xB8, 0x73, 0x72, 
    0x7D, 0x50, 0xA5, 0x81, 0x26, 0x5C, 0x8F, 0x94, 0xEA, 0x73, 0x59, 0x5C, 0x33, 0xF9, 0xC3, 0x65, 
    0x1E, 0x92, 0xCD, 0x20, 0xC3, 0xBF, 0xD7, 0x8A, 0xCF, 0xCC, 0xD0, 0x61, 0xF8, 0xFB, 0x1B, 0xF4, 
    0xB6, 0x0F, 0xD4, 0xCF, 0x3E, 0x55, 0x48, 0x4C, 0x99, 0x2D, 0x40, 0x44, 0x7C, 0xBA, 0x7B, 0x6F, 
    0xDB, 0x5D, 0x71, 0x91, 0x2D, 0x93, 0x80, 0x19, 0xE3, 0x26, 0x5D, 0x59, 0xBE, 0x46, 0x6D, 0x90, 
    0x4B, 0xDF, 0x72, 0xCE, 0x6C, 0x69, 0x72, 0x8F, 0x5B, 0xA4, 0x74, 0x50, 0x2A, 0x42, 0x95, 0xB2, 
    0x19, 0x04, 0x88, 0xD7, 0xDA, 0xBB, 0x17, 0x23, 0x69, 0xF4, 0x52, 0xEB, 0xC8, 0x55, 0xBE, 0xBC, 
    0x2E, 0xA9, 0xD0, 0x57, 0x7D, 0xC6, 0xC8, 0x8B, 0x86, 0x7B, 0x73, 0xCD, 0xE4, 0x32, 0x79, 0xC0, 
    0x75, 0x53, 0x53, 0xE7, 0x59, 0x38, 0x0A, 0x8C, 0xEC, 0x06, 0xA9, 0xFC, 0xA5, 0x15, 0x81, 0x61, 
    0x3E, 0x44, 0xCD, 0x05, 0xF8, 0x54, 0x04, 0x00, 0x79, 0xB2, 0x0D, 0x69, 0x2A, 0x47, 0x60, 0x1A, 
    0x2B, 0x79, 0x3D, 0x4B, 0x50, 0x8A, 0x31, 0x72, 0x48, 0xBB, 0x75, 0x78, 0xD6, 0x35, 0x90, 0xE1,
};

static HI_U8 NO_PADDING[] =
{
    0x31, 0x5d, 0xfa, 0x52, 0xa4, 0x93, 0x52, 0xf8, 0xf5, 0xed, 0x39, 0xf4, 0xf8, 0x23, 0x4b, 0x30, 
    0x11, 0xa2, 0x2c, 0x5b, 0xa9, 0x8c, 0xcf, 0xdf, 0x19, 0x66, 0xf5, 0xf5, 0x1a, 0x6d, 0xf6, 0x25, 
    0x89, 0xaf, 0x06, 0x13, 0xdc, 0xa4, 0xd4, 0x0b, 0x3c, 0x1c, 0x4f, 0xb9, 0xd3, 0xd0, 0x63, 0x29, 
    0x2a, 0x5d, 0xfe, 0xb6, 0x99, 0x20, 0x58, 0x36, 0x2b, 0x1d, 0x57, 0xf4, 0x71, 0x38, 0xa7, 0x8b, 
    0xad, 0x8c, 0xef, 0x1f, 0x2f, 0xea, 0x4c, 0x87, 0x2b, 0xd7, 0xb8, 0xc8, 0xb8, 0x09, 0xcb, 0xb9, 
    0x05, 0xab, 0x43, 0x41, 0xd9, 0x75, 0x36, 0x4d, 0xb6, 0x8a, 0xd3, 0x45, 0x96, 0xfd, 0x9c, 0xe8, 
    0x6e, 0xc8, 0x37, 0x5e, 0x4f, 0x63, 0xf4, 0x1c, 0x18, 0x2c, 0x38, 0x79, 0xe2, 0x5a, 0xe5, 0x1d, 
    0x48, 0xf6, 0xb2, 0x79, 0x57, 0x12, 0xab, 0xae, 0xc1, 0xb1, 0x9d, 0x11, 0x4f, 0xa1, 0x4d, 0x1b, 
    0x4c, 0x8c, 0x3a, 0x2d, 0x7b, 0x98, 0xb9, 0x89, 0x7b, 0x38, 0x84, 0x13, 0x8e, 0x3f, 0x3c, 0xe8, 
    0x59, 0x26, 0x90, 0x77, 0xe7, 0xca, 0x52, 0xbf, 0x3a, 0x5e, 0xe2, 0x58, 0x54, 0xd5, 0x9b, 0x2a, 
    0x0d, 0x33, 0x31, 0xf4, 0x4d, 0x68, 0x68, 0xf3, 0xe9, 0xb2, 0xbe, 0x28, 0xeb, 0xce, 0xdb, 0x36, 
    0x1e, 0xae, 0xb7, 0x37, 0xca, 0xaa, 0xf0, 0x9c, 0x6e, 0x27, 0x93, 0xc9, 0x61, 0x76, 0x99, 0x1a, 
    0x0a, 0x99, 0x57, 0xa8, 0xea, 0x71, 0x96, 0x63, 0xbc, 0x76, 0x11, 0x5c, 0x0c, 0xd4, 0x70, 0x0b, 
    0xd8, 0x1c, 0x4e, 0x95, 0x89, 0x5b, 0x09, 0x17, 0x08, 0x44, 0x70, 0xec, 0x60, 0x7c, 0xc9, 0x8a, 
    0xa0, 0xe8, 0x98, 0x64, 0xfa, 0xe7, 0x52, 0x73, 0xb0, 0x04, 0x9d, 0x78, 0xee, 0x09, 0xa1, 0xb9, 
    0x79, 0xd5, 0x52, 0x4f, 0xf2, 0x39, 0x1c, 0xf7, 0xb9, 0x73, 0xe0, 0x3d, 0x6b, 0x54, 0x64, 0x86
};

HI_S32 PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_E enScheme, HI_U8 *pu8Expect)
{
	HI_S32 ret = HI_SUCCESS;
    HI_U8  u8Enc[256];
    HI_U8  u8Dec[256];
    HI_U32 u32OutLen;
    HI_UNF_CIPHER_RSA_PUB_ENC_S stRsaEnc;
    HI_UNF_CIPHER_RSA_PRI_ENC_S stRsaDec;

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
		return HI_FAILURE;
	}

    memset(&stRsaEnc, 0, sizeof(HI_UNF_CIPHER_RSA_PUB_ENC_S));
    memset(&stRsaDec, 0, sizeof(HI_UNF_CIPHER_RSA_PRI_ENC_S));

    stRsaEnc.enScheme = enScheme;
    stRsaEnc.stPubKey.pu8N = N;
    stRsaEnc.stPubKey.pu8E = E;
    stRsaEnc.stPubKey.u16NLen = sizeof(N);
    stRsaEnc.stPubKey.u16ELen = sizeof(E);
    if(enScheme == HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING)
    {
        ret = HI_UNF_CIPHER_RsaPublicEnc(&stRsaEnc, NO_PADDING, 256, u8Enc, &u32OutLen);
    }
    else
    {
        ret = HI_UNF_CIPHER_RsaPublicEnc(&stRsaEnc, test_data, sizeof(test_data) - 1, u8Enc, &u32OutLen);
    }

    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPublicEnc failed\n");
		return HI_FAILURE;
	}

    if(pu8Expect != HI_NULL)
    {
        if(memcmp(u8Enc, pu8Expect, u32OutLen) != 0)
    	{
    	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPublicEnc failed\n");
            printBuffer("enc", u8Enc, u32OutLen);
            printBuffer("expect", pu8Expect, 256);
    		return HI_FAILURE;
    	}
    }
   
    stRsaDec.enScheme = enScheme;
    stRsaDec.stPriKey.pu8N = N;
    stRsaDec.stPriKey.pu8D = D;
    stRsaDec.stPriKey.u16NLen = sizeof(N);
    stRsaDec.stPriKey.u16DLen = sizeof(D);

    ret = HI_UNF_CIPHER_RsaPrivateDec(&stRsaDec, u8Enc, u32OutLen, u8Dec, &u32OutLen);
    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaVerify failed\n");
		return HI_FAILURE;
	}


    if(enScheme == HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING)
    {
        if(memcmp(u8Dec, NO_PADDING, 256) != 0)
        {
            HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPrivateDec failed\n");
            printBuffer("dec", u8Dec, u32OutLen);
            printBuffer("expect", NO_PADDING, 256);
            return HI_FAILURE;
        }
    }
    else
    {
        if((sizeof(test_data) - 1) != u32OutLen)
        {
            HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPrivateDec len error\n");
            printf("dec: 0x%x, expect: 0x%x\n", u32OutLen, sizeof(test_data) - 1);
            return HI_FAILURE;
        }
        if(memcmp(u8Dec, test_data, sizeof(test_data) - 1) != 0)
        {
            HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPrivateDec failed\n");
            printBuffer("enc", u8Enc, u32OutLen);
            printBuffer("expect", test_data, sizeof(test_data) - 1);
            return HI_FAILURE;
        }
    }

    printf("sample %s run successfully!\n", __FUNCTION__);
    
    HI_UNF_CIPHER_DeInit();

    return HI_SUCCESS;
}

HI_S32 PKCS_PRI_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_E enScheme)
{
	HI_S32 ret = HI_SUCCESS;
    HI_U8  u8Sign[256];
    HI_U32 u32SignLen;
    HI_U8  u8Hash[32];
    HI_U32 u32HLen;
    HI_UNF_CIPHER_RSA_PRI_ENC_S stRsaEnc;
    HI_UNF_CIPHER_RSA_PUB_ENC_S stRsaDec;

	ret = HI_UNF_CIPHER_Init();
	if ( HI_SUCCESS != ret )
	{
		return HI_FAILURE;
	}

    memset(&stRsaEnc, 0, sizeof(HI_UNF_CIPHER_RSA_PRI_ENC_S));
    memset(&stRsaDec, 0, sizeof(HI_UNF_CIPHER_RSA_PUB_ENC_S));
    stRsaEnc.enScheme = enScheme;
    stRsaDec.enScheme = enScheme;
    if (enScheme == HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1)
    {
        printf("Generate a RSA KEY...\n");
        stRsaEnc.stPriKey.pu8N = (HI_U8*)malloc(256);
        stRsaEnc.stPriKey.pu8E = (HI_U8*)malloc(256);
        stRsaEnc.stPriKey.pu8D = (HI_U8*)malloc(256);
        ret = HI_UNF_CIPHER_RsaGenKey(2048, 0x10001, &stRsaEnc.stPriKey);
        if ( HI_SUCCESS != ret )
    	{
    	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaGenKey failed\n");
    		return HI_FAILURE;
    	}
        stRsaDec.stPubKey.pu8N = stRsaEnc.stPriKey.pu8N;
        stRsaDec.stPubKey.pu8E = stRsaEnc.stPriKey.pu8E;
        stRsaDec.stPubKey.u16NLen = stRsaEnc.stPriKey.u16NLen;
        stRsaDec.stPubKey.u16ELen = stRsaEnc.stPriKey.u16ELen;
        printBuffer ("N", stRsaEnc.stPriKey.pu8N, stRsaEnc.stPriKey.u16NLen);
        printBuffer ("E", stRsaEnc.stPriKey.pu8E, stRsaEnc.stPriKey.u16ELen);
        printBuffer ("D", stRsaEnc.stPriKey.pu8D, stRsaEnc.stPriKey.u16DLen);
#ifdef CIPHER_KLAD_SUPPORT
        ret = HI_UNF_CIPHER_KladEncryptKey(HI_UNF_CIPHER_KEY_SRC_EFUSE_1, HI_UNF_CIPHER_KLAD_TARGET_RSA, 
            stRsaEnc.stPriKey.pu8D, stRsaEnc.stPriKey.pu8D, stRsaEnc.stPriKey.u16DLen);
        if(HI_SUCCESS != ret)
    	{
    	    HI_ERR_CIPHER("Error: Klad Encrypt Key failed!\n");
    		return HI_FAILURE;	
    	}
        printBuffer("ENC_D", stRsaEnc.stPriKey.pu8D, stRsaEnc.stPriKey.u16DLen);
        stRsaEnc.enKeySrc = HI_UNF_CIPHER_KEY_SRC_KLAD_1;
#endif
    }
    else
    {
        stRsaEnc.stPriKey.pu8N = N;
        stRsaEnc.stPriKey.pu8D = D;
        stRsaEnc.stPriKey.u16NLen = sizeof(N);
        stRsaEnc.stPriKey.u16DLen = sizeof(D);
        stRsaDec.stPubKey.pu8N = N;
        stRsaDec.stPubKey.pu8E = E;
        stRsaDec.stPubKey.u16NLen = sizeof(N);
        stRsaDec.stPubKey.u16ELen = sizeof(E);
        stRsaEnc.enKeySrc = HI_UNF_CIPHER_KEY_SRC_USER;
    }
    ret = HI_UNF_CIPHER_RsaPrivateEnc(&stRsaEnc, sha256_sum, 32, u8Sign, &u32SignLen);
    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaSign failed\n");
		return HI_FAILURE;
	}

    switch(enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        break;
    default:
        break;
    }

 //   printBuffer("sign", u8Sign, u32SignLen);
    
    ret = HI_UNF_CIPHER_RsaPublicDec(&stRsaDec, u8Sign, u32SignLen, u8Hash, &u32HLen);
    if ( HI_SUCCESS != ret )
	{
	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaVerify failed\n");
		return HI_FAILURE;
	}

    switch(enScheme)
    {
    case HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1:
        if(memcmp(u8Hash, sha256_sum, u32HLen) != 0)
    	{
    	    HI_ERR_CIPHER("HI_UNF_CIPHER_RsaPublicDec failed\n");
            printBuffer("sign", u8Sign, u32SignLen);
            printBuffer("sha256_sum", sha256_sum, sizeof(sha256_sum));
    		return HI_FAILURE;
    	}
        break;
    default:
        break;
    }

    if (enScheme == HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1)
    {
        free(stRsaEnc.stPriKey.pu8N);
        free(stRsaEnc.stPriKey.pu8E);
        free(stRsaEnc.stPriKey.pu8D);
    }
    printf("\033[0;1;32m""sample PKCS_PRI_ENC run successfully!\n""\033[0m");
    
    HI_UNF_CIPHER_DeInit();

    return HI_SUCCESS;
}

int sample_rsa_enc()
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_NO_PADDING, HI_NULL);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_2, HI_NULL);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA1, HI_NULL);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_OAEP_SHA256, HI_NULL);
    if (s32Ret != HI_SUCCESS)
	{
        return s32Ret;
    }

    s32Ret = PKCS_PUB_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_RSAES_PKCS1_V1_5, HI_NULL);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = PKCS_PRI_ENC(HI_UNF_CIPHER_RSA_ENC_SCHEME_BLOCK_TYPE_1);
    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }
    
    return HI_SUCCESS;
}
#else
int sample_rsa_enc()
{
    printf("RSA not support!!!\n");
    return HI_SUCCESS;
}
#endif

