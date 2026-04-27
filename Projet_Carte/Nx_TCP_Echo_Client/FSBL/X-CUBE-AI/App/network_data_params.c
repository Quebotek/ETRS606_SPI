/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-04-26T23:20:27+0200
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[451] = {
  0xbe525c78be21d610U, 0x3d9824213d7370ceU, 0x3e7a4f48bedfbf7dU, 0xbd8827e83dfe8f25U,
  0xbe88db30be83b9d9U, 0x3d91b38cbdfef787U, 0x3dc2fa993f06ead4U, 0xbeaea225bce863baU,
  0x3e44d3d03e3e5babU, 0x3cda62e63e8ace5dU, 0xbea912bbbe89879fU, 0xbed0f4303db2c6ccU,
  0xbed309f7bf1b2eb3U, 0x3f03909e3dfd956bU, 0x3d9005033ed2cf76U, 0x3e9eb56bbda04a99U,
  0x3e424bc53ecfae63U, 0x3ef69d05ba1394c9U, 0xbe43d79d3d07ae10U, 0xbe02c365be3a8052U,
  0x3a83ad2b3e596da5U, 0x3cd9202fbd553777U, 0x3ecd71b0bee0db24U, 0x3d908ea6bcd560b7U,
  0x3ea88c1b3e845e55U, 0x3ebe4c13be9138bbU, 0x3e6293fd3e3d520fU, 0x3f34edc13ed7825dU,
  0xbedebbd03e123bfcU, 0xbead58a0be260128U, 0x3e1bb672bd0677d3U, 0xbe8abe95bf0907a5U,
  0x3d0a2523be963d52U, 0xbe70fca83e2de9e8U, 0xbdc466e73df48f62U, 0x3e5970583eddb73cU,
  0x3e99de9c3edda13cU, 0x3ec1075ebe1c8cb1U, 0xbde90274bde37db4U, 0xbe023dd03dd643f5U,
  0xbee4c86f3ec8477fU, 0x3de0df8db9c0b301U, 0xbeca38e73d4eaddfU, 0x3f13371ebf1d8cf9U,
  0xbe0d30e5be8aa6adU, 0xbec04827bd7d8fabU, 0xbf0b8c32bead7807U, 0xbeeb51433db706c8U,
  0x3dae25183e88fb82U, 0xbe456231becbfb07U, 0x3c212e9c3e440c02U, 0x3f1e91d73e8c18f3U,
  0xbe15d03e3a116e02U, 0xbc4e57793e3a661eU, 0xbea133b2bef15ca2U, 0xbf132270bdd748d6U,
  0x3dc1c3de3e25c4b2U, 0x3ef29ea3bed00355U, 0xbe2591033ebf26e9U, 0xbe04f3903f2999afU,
  0x3e344aecbec56631U, 0x3aca6cc83eb5f1a0U, 0xbed6ca5fbe7e91c2U, 0xbebc003dbd788f6eU,
  0x3e64c72d3f09649cU, 0x3e21b696becddf59U, 0x3eaf3abc3e429bddU, 0xbe86d4dd3ef22fa9U,
  0x3e8c962ebe53220fU, 0x3e19c314bd970fe9U, 0x3eb4508b3e604e15U, 0xbdb2e193becfa969U,
  0x3e3f59463e1a12daU, 0xbecd185d3e9df7a1U, 0xbeb50bf03d5dc348U, 0xbeaa6f78be0a72ffU,
  0xbeac8a3dbe3c9219U, 0xbd9a257d3f1997fdU, 0x3e1c2f90bd9686bbU, 0xbe182e653c8080daU,
  0xbeacc9f2beaea1b1U, 0x3e290b79be8e48bbU, 0x3eceeca9bdd1ac15U, 0x3f146b3dbf4fd7f0U,
  0xbe84446d3e84da8aU, 0xbe7a4af5bea2762dU, 0x3eb699013f04b303U, 0xbd3ace38bec6016cU,
  0x3e99693dbed465b3U, 0x3bad85cb3f062ae8U, 0x3def4b9f3db6d522U, 0xbe8a7cd5bd9c33f6U,
  0x3cb7fec5be01948fU, 0x3dfc20243e54cc2bU, 0x3e08256cbd9980bbU, 0x3ee0a60bbf1f750fU,
  0xbe62bd75bdbb42aeU, 0x3eff6545bdb30efcU, 0x3e41c60f3acdd734U, 0xbec6c63d3e21480aU,
  0xbecbf677be56cb22U, 0x3e43d432be7b440bU, 0xbca5a4b0be767cacU, 0xbf28302ebeb5e237U,
  0x3f25148c3ea4193eU, 0xbd87987abe2a74c3U, 0x3dcdc04f3dde42c1U, 0x3dd47c3e3d13d984U,
  0x3e8bea3bbd9526c4U, 0x3d18f4c23f3bc778U, 0xbd19d63abdb8c157U, 0x3d996c483dc00c53U,
  0xbe46b4c3bde0f50aU, 0x3e14de5f3f1bc9c4U, 0x3e4915093e10a886U, 0x3d792033bf343445U,
  0x3e81edecbe7071f9U, 0x3eccdf7ebe9040c5U, 0x3e7f48fe3ee12c79U, 0xbf0946613def7758U,
  0xbe2a82973ecc7caeU, 0xbd57dd3b3e1dea9aU, 0x3eeda54d3e894b90U, 0x3ee4d0403e3c2ec9U,
  0x3eae93643dcca12dU, 0xbc4991083f07ac5fU, 0xbe05b713bd9c7a83U, 0x3e92bddf3eda1916U,
  0x3e81f9a53e4680dcU, 0xbb569e7a3e7bf692U, 0x3eb0d6c23e0175f8U, 0xbe8d943cbdaa64d8U,
  0x3e72b2053d2c290eU, 0x3daef01c3e37b068U, 0x3ea32cca3e69058bU, 0xbe5af0853e8860f4U,
  0xbcb26cf2be88e83bU, 0xbec459683dedac04U, 0x3e83fba83e54959aU, 0xbd87dac2be9c79ffU,
  0xbc29068cbd853e80U, 0xbf0952eabd99fadbU, 0x3ee621ddbe1ce2d2U, 0x3de034f33d8722ebU,
  0xbe03efddbe3aa34fU, 0xbebcc3613e98f921U, 0x3eb869f43dfd6dc5U, 0xbd9728d3bf10b1a1U,
  0xbda964b7be03802eU, 0xbdd395aabba4fadbU, 0x3ef63ff2be3b598dU, 0x3eb2d5f13dc466b3U,
  0xbcbb88f7beb8fdc1U, 0x3cc6de813ea35835U, 0xbe5db6913f0f6c66U, 0xbe225ee2bdf2d1f1U,
  0xbed309c5be9ce3c1U, 0x3c94ff0a3e8b5b5fU, 0xbe5783053de4a3eaU, 0xbd5e070c3d03b4a9U,
  0x3d207fdf3dfdf626U, 0x3f110056bde8e188U, 0x3e8051363e0afaefU, 0xbf22a343be9253d5U,
  0x3ef07b423e2689ecU, 0xbd0afdf4bdba80edU, 0xbe9ba27abdb03296U, 0x3e8268783e90e256U,
  0x3e54dbf63eee6bacU, 0x3ef89763be928e39U, 0xbde3ef543ea1f368U, 0x3e049a3e3f0f4107U,
  0xbe8d89553e2170e1U, 0x3f2feff33db9617bU, 0x3e77da423f178b78U, 0x3e82f65abe8420f9U,
  0x3cb5d47dbe24ece6U, 0xbe5df838be03f756U, 0x3e0735713e0b6808U, 0xbe9f11edbf10891dU,
  0x3e0093033e675c14U, 0xbed9338e3ebe3fecU, 0x3dc8629f3e40b0fdU, 0xbdbf7e853f0c094dU,
  0x3edd795fbe92e168U, 0xbdcfecc3beb544fcU, 0xbe4c8e9c3ee4fa9eU, 0x3f0c93dc3beb8a04U,
  0x3e860ef43f0b0680U, 0xbef56c61bee6c0f8U, 0x3f0e5b043ed5f687U, 0x3e315dacbdee82d4U,
  0xbc81a356bc0ace09U, 0xbea9ce82be93b09bU, 0xb91a07f03e74c951U, 0x3d73eed63e19593eU,
  0x3e93d53ebe6cab81U, 0xbeb0beaf3eb3fa3dU, 0xbe53b7133f1dfa07U, 0xbdef550f3eacc4bbU,
  0x3ecfac6a3f292aa7U, 0x3e00226bbef7fab1U, 0xbe459a633de3c502U, 0x3d926a7f3f02e24eU,
  0x3d3387c23d639491U, 0xbeb61778be075cb5U, 0x3de0eeb73e2582a6U, 0xbef2d619bf3e5a9aU,
  0xbe0e84fc3e6c44d7U, 0xbc03eb7a3eaeb17aU, 0xbe9185d73e607d70U, 0x3e89bd09bcb64c28U,
  0x3e806b2e3e6e4fccU, 0x3e703ebbbd319dbcU, 0x3f0a753ebeed524eU, 0x3f10ff6abd20bf8cU,
  0xbdfe4539be8c5a9aU, 0xbe52bdbdbda35a1eU, 0x3ced3946bf3c1bbdU, 0x3e292af63ea5dfe7U,
  0xbe0ff3ba3dc958c2U, 0x3e21b39bbf29f874U, 0xbeb0aa86be2d217bU, 0x3e9b73213be178bcU,
  0x3d7f6f643e4e0919U, 0xbdee7c353d36aea4U, 0x3e8cf649bea9a907U, 0x3e8131dabdd712a9U,
  0x3de516773eeb60d8U, 0xbe9e5dd23ea9a550U, 0x3ec60a2f3e8cd0a5U, 0xbe8f06fabe59fdb7U,
  0x3e5c87fdbe35ac67U, 0xbde4d9933cb307c4U, 0x3eaf34ca3d1b797eU, 0xbec92eb03d41ad55U,
  0x3f0f40eb3e4fc7e2U, 0xbe2f8b993e4e75d7U, 0x3b0c78da3e500cd9U, 0xbeefc43a3e10e0c7U,
  0xbe5749ed3ea1fdd8U, 0xbf0c5644bdeae0b0U, 0xbdd5f255be3a62dbU, 0xbf090d76be7a8e3aU,
  0xbe4b8a4e3f1f3945U, 0xbe7dd70d3dd04576U, 0x3db5c1c9bec6706bU, 0x3f1259f13ed3ecf5U,
  0xbe55f72e3d47ca87U, 0xbe49d7983e9956efU, 0x3ea10954be209e80U, 0xbd85c8423eef55dcU,
  0xbe445632bc6a4f37U, 0x3f39ca2a3d29ca5cU, 0x3e4b2c8d3eb3feb1U, 0x3f2843243e669705U,
  0x3e73ab58bea76b54U, 0xbd5ece59bf105f9cU, 0xbdf8a964bfa754b5U, 0x3ebdfe823e6df2b2U,
  0xbfdf58ddbedb893dU, 0x3e5d9ba9be164c25U, 0x3de40b88bedf3d8fU, 0x3e7b7571be81a42bU,
  0xbe07e8f5bf5dde61U, 0xbed7da88bee81530U, 0x3f2dd1f23e8e5eb5U, 0xbe50d2aebe9d2ae0U,
  0x3f3ac3843e5d6fb9U, 0xbec68f5dbea1283aU, 0xbc029d64be36f1deU, 0xbeb8ee333e3aee00U,
  0x3f00f4d13ec5d566U, 0xbe8c2e6bbd6599cdU, 0x3ef426f13e441b12U, 0xbe1b32f3beb63d0eU,
  0xbb5ef2f73ea9cf03U, 0x3d92a1243ebcca8bU, 0x3e55b69e3e680a98U, 0xbe886d053e675051U,
  0xbd3d923bbe47dc82U, 0xbea00f233e5ba34fU, 0x3ec121f93d417947U, 0x3c31e44dbd34ea3fU,
  0x3f50cba53d4520bdU, 0xbefdcb43beb5d0c7U, 0x3e308093bd7458d4U, 0xbe662d1f3dda5dccU,
  0xbd9337383db9a186U, 0x3e14aa773ee58dc8U, 0x3eff14a63e7811acU, 0x3eb158e13f417f72U,
  0x3f1549373ba9c4dcU, 0x3d6650f03eb4dfd6U, 0xbe8f6c0abda31bd7U, 0x3e1806f83d7623dfU,
  0x3e5dd6a63e8eb562U, 0xbed73f38be5d501bU, 0x3effdbd03da447d5U, 0xbe71ea4fbf42e5edU,
  0xbcd0be02befaa571U, 0xbf224dc33ebf79b1U, 0xbebaa8163d31eadaU, 0x3e8af9803e5eb374U,
  0x3ecb11123ea96270U, 0x3d6c11bf3bffffb2U, 0x3e7109693eab18e1U, 0x3cd9744c3d97d097U,
  0x3e8fd3fbbde42b73U, 0x3e4db5cc3ec5e4a7U, 0xbde666f6be0a39b2U, 0xbf29608f3f0737d3U,
  0xbd2e0052be2fcb3eU, 0xbe75fbdfbe2f6841U, 0x3ee493273eef4385U, 0xbeb81c6bbecf749cU,
  0x3e9ad032be153960U, 0xbe9e3199bd38cc4cU, 0x3e727b8ebe046296U, 0x3db5b260bd9d6549U,
  0xbd1e3d113e28db3dU, 0xbe3080283e75e027U, 0x3dcca2323ea80a27U, 0xbe9c998f3c71ddd0U,
  0xbd9c2ced3dd4ce5bU, 0x3d2b1b773eba7dfaU, 0x3d9fbf52be3eec9aU, 0xbea6f1283dcae5dfU,
  0xbea239dabdd30cc5U, 0xbf0cdb6c3e8b95a3U, 0x3e0363283d8a0ae3U, 0x3eea8dc5be2b6a9eU,
  0x3df0ba6f3eac323cU, 0xbf4d042cbdc80323U, 0x3e17fe3bbc89659aU, 0x3d9c9f01bd46a074U,
  0x3ecca4dd3db41be8U, 0xbf8da2d33de338a5U, 0x3e949ef4bea20680U, 0x3e9a4397bf108fd6U,
  0xbe446a6abe8b040bU, 0xbee41b6abe32b03bU, 0x3e982bf4beeaf25eU, 0xbe588facbe43918aU,
  0x3f08d15fbe304d4eU, 0xbe010804be837d15U, 0x3e30243abefb5726U, 0xbdabeef93e6de7a4U,
  0x3e795c21be1554e8U, 0x3da710573ea9351aU, 0xbd94ae2d3e9181c9U, 0x3eb34581bef35e60U,
  0xbd0244dbbc0d43bbU, 0xbdc40f463c1cfefcU, 0x3c87ddf7bece758cU, 0xbe8b1e983ea4391eU,
  0xbda4dc143e46965dU, 0xbd2cc6cc3eb011acU, 0x3e3a19603e9f9924U, 0x3da7d5033d9691a3U,
  0x3c7cc96ebe0043f9U, 0xbf1bc48ebdb61e0cU, 0xbe41a38f3e908bbcU, 0x3ec19cb9bebefcbaU,
  0xbeda6c403d814a67U, 0xbe3f3eb63db24c42U, 0xbeff13aa3e50e453U, 0x3dd6eea13ec57342U,
  0x3eb72956be205858U, 0xbd0603693ed934c1U, 0xbda7dfb5bb7d6127U, 0x3eb3e7793f2b0a93U,
  0xbe267de33e6e31deU, 0x3eb775073dbf77b3U, 0x3e963f15bed62aa3U, 0x3e040441bcba13d3U,
  0xbd7a7df13d603990U, 0xbe934e6d3e0c68d9U, 0x3c970ad33e69551fU, 0x3d442c85beb0f9a4U,
  0xbe93182dbedd7565U, 0x3edaf1693ea85a2dU, 0xbd95f053be38041fU, 0x3f0c7e963f305f83U,
  0x3deb84543e309539U, 0x3e870903bd2a98e3U, 0xbec25ff7bee9be0cU, 0x3e6f22fabf14a903U,
  0x3db967bc3e87a99cU, 0x3eb0ac753c62a795U, 0x3e8f93fdbe279b1cU, 0x3d8d741bbe7cfa5dU,
  0xbf376f503e8c9738U, 0x3f21ab413ed58eb8U, 0x3ba66f7b3ee2dbfbU, 0xbecd715a3e61d043U,
  0x3f093d8dbf534a47U, 0x3ee68645bc9306ecU, 0xbf31080dbe5128beU, 0xbe974a5a3e1f5f47U,
  0xbc8e13e03e2c512eU, 0x3d80a05c3e4e01caU, 0x3e9e9d283d5cf0d0U, 0x3bb3ba723e7a390fU,
  0x3d56aa0d3ed1cce7U, 0x3e62d6513e1ea1d1U, 0x3ca38b0c3e28cab2U, 0xbe136dc93e2e8b37U,
  0x3de2d115bc49e3fcU, 0x3eaeaf823ec6b0b8U, 0x3e11778fbf840c93U, 0xbe750ca3befef225U,
  0xbc6f94e03f0f81f1U, 0x3dd131c23eb5837dU, 0xbf18ba8f3e8bf15eU, 0xbf34f93dbf313713U,
  0xbec75d13be62e426U, 0xbf3042e63eddadc4U, 0xbe0cb74a3eb932fdU, 0xbf03ac033cbf5c4fU,
  0x3e8038ab3eb88e07U, 0x3ee21bb23ea48a52U, 0x3e0749173f2b44a9U, 0xbedf283ebcaf6a5fU,
  0xbf1cd5b33f1f51f3U, 0xbe972b11be349141U, 0x3e3aab4a3e4f3855U, 0x3f4e23c13ef2b8beU,
  0xbe8f77e73ed02d83U, 0xbe5699bdbf160610U, 0x3ef32974be95a9e2U, 0xbf3458cd3ee6ac35U,
  0x3f320496bf3f2f71U, 0xbeca084b3f18a6daU, 0xbf6a3088be5440acU, 0x3e77c6fa3ea15345U,
  0xbf793da5bf7589a7U, 0x3de901c5bf2231c4U, 0xbebf09aebf02960aU, 0x3e10fa2abc802d02U,
  0xbd810900bedc01dcU, 0xbed30210bfa1fcc7U, 0xbe96d6a63eb6e522U, 0x3f88603cbf6d626bU,
  0x3f3c0713bf614177U, 0xbf334816bdda2693U, 0xbf05ac023ed76986U, 0x3f391bdf3f082311U,
  0x3e039b7b3d0a78baU, 0xbe0eb7e63d9a5b62U, 0xbe66dec8U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

