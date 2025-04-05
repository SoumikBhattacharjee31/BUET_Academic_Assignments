/*
 * Copyright (c) 2014 Natale Patriciello, <natale.patriciello@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 */

#ifndef TCPHIGHSPEEDnew_H
#define TCPHIGHSPEEDnew_H

#include "tcp-congestion-ops.h"

namespace ns3
{

class TcpSocketState;

/**
 * \ingroup congestionOps
 *
 * \brief An implementation of TCP HighSpeednew
 *
 * TCP HighSpeednew is designed for high-capacity channels or, in general, for
 * TCP connections with large congestion windows.
 * Conceptually, with respect to the standard TCP, HighSpeednew makes the
 * cWnd grow faster during the probing phases and accelerates the
 * cWnd recovery from losses.
 * This behavior is executed only when the window grows beyond a
 * certain threshold, which allows TCP Highspeednew to be friendly with standard
 * TCP in environments with heavy congestion, without introducing new dangers
 * of congestion collapse.
 * At the core of TCP HighSpeednew there are two functions, a(w) and b(w), which respectively
 * specify the cWnd growth addendum and the cWnd reduction factor when
 * given an actual cWnd value w.
 *
 * More information: http://dl.acm.org/citation.cfm?id=2756518
 */
class TcpHighSpeednew : public TcpNewReno
{
  public:
    /**
     * \brief Get the type ID.
     * \return the object TypeId
     */
    static TypeId GetTypeId();

    /**
     * Create an unbound tcp socket.
     */
    TcpHighSpeednew();

    /**
     * \brief Copy constructor
     * \param sock the object to copy
     */
    TcpHighSpeednew(const TcpHighSpeednew& sock);
    ~TcpHighSpeednew() override;

    std::string GetName() const override;

    uint32_t GetSsThresh(Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight) override;

    Ptr<TcpCongestionOps> Fork() override;

    /**
     * \brief Lookup table for the coefficient a (from RFC 3649)
     *
     * \param w Window value (in packets)
     *
     * \return the coefficient a
     */
    static uint32_t TableLookupA(uint32_t w);

    /**
     * \brief Lookup table for the coefficient b (from RFC 3649)
     *
     * \param w Window value (in packets)
     *
     * \return the coefficient b
     */
    static double TableLookupB(uint32_t w);

  protected:
    void CongestionAvoidance(Ptr<TcpSocketState> tcb, uint32_t segmentsAcked) override;

  private:
    uint32_t m_ackCnt; //!< Number of received ACK, corrected with the coefficient a
};

} // namespace ns3

#endif // TCPHIGHSPEEDnew_H
