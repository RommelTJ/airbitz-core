/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */
/**
 * @file
 * Airbitz general, non-account-specific server-supplied data.
 *
 * The data handled in this file is basically just a local cache of various
 * settings that Airbitz would like to adjust from time-to-time without
 * upgrading the entire app.
 */

#ifndef ABCD_GENERAL_HPP
#define ABCD_GENERAL_HPP

#include "bitcoin/Typedefs.hpp"
#include <map>
#include <vector>

namespace abcd {

/**
 * Maps from transaction sizes to corresponding fees.
 */
//typedef std::map<size_t, uint64_t> BitcoinFeeInfo;

/**
 * New Bitcoin fee information.
 */
struct BitcoinFeeInfo
{
    /**
     * Fee per KB aimed at getting approved within n confirmations,
     * where n is the index into this array. Index 0 is unused
     */
    double confirmFees[7];

    /**
     * Target number of blocks to get transaction confirmed when set to Low
     */
    unsigned int lowFeeBlock;

    /**
     * Target number of blocks to get transaction confirmed when set to Standard
     * for low value transactions
     */
    unsigned int standardFeeBlockLow;

    /**
     * Target number of blocks to get transaction confirmed when set to Standard
     * for high value transactions
     */
    unsigned int standardFeeBlockHigh;

    /**
     * Target number of blocks to get transaction confirmed when set to High
     */
    unsigned int highFeeBlock;

    /**
     * The percentage of the outgoing funds that we will try to send as a mining fee
     * ie. If outgoing funds = 1 BTC and targetFeePercentage = 0.2, then we will try
     * to send 0.002 BTC as the per KB mining fee. Note that we will not go below
     * the confirmFees3 value. We will cap the fees to confirmFees2 unless the user chooses
     * to send a "High" fee in which case we will use confirmFees1 regardless
     * of amount of outgoing funds.
     */
    double targetFeePercentage;
};


/**
 * Airbitz transaction fee information.
 */
struct AirbitzFeeInfo
{
    AddressSet addresses;

    // Fee amounts for incoming funds:
    double incomingRate;
    int64_t incomingMax;
    int64_t incomingMin;

    // Fee amounts for outgoing funds:
    double outgoingRate;
    int64_t outgoingMax;
    int64_t outgoingMin;
    int64_t noFeeMinSatoshi;

    // When to actually send fees:
    int64_t sendMin;
    time_t sendPeriod;
    std::string sendCategory;
    std::string sendPayee;
};

/**
 * Downloads general info from the server if the local file is out of date.
 */
Status
generalUpdate();

/**
 * Returns true if the estimated fees should be redownloaded from a Stratum
 * server. Occurs if estimated fees file is out of date or doesn't exist.
 */
bool
generalEstimateFeesNeedUpdate();

/**
 * Updates the cached estimated fees with the specified fee.
 * Value is denoted as the fee amount in BTC / kB to gain a confirmation
 * in 'blocks' number of blocks.
 */
Status
generalEstimateFeesUpdate(size_t blocks, double fee);

/**
 * Obtains the Bitcoin mining fee information.
 * The returned table always has at least one entry.
 */
BitcoinFeeInfo
generalBitcoinFeeInfo();

/**
 * Obtains the Airbitz fee information.
 * The fees will be zero if something goes wrong.
 */
AirbitzFeeInfo
generalAirbitzFeeInfo();

/**
 * Obtains a list of libbitcoin servers for the current network
 * (either testnet or mainnet).
 * Returns a fallback server if something goes wrong.
 */
std::vector<std::string>
generalBitcoinServers();

/**
 * Obtains a list of sync servers.
 * Returns a fallback server if something goes wrong.
 */
std::vector<std::string>
generalSyncServers();

} // namespace abcd

#endif
