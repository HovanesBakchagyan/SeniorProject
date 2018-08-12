//
// Created by xaqq on 7/1/15.
//
#pragma once

#include "mifareomnikeyxx21commands.hpp"
#include "mifareplus_pcsc_sl1.hpp"

namespace logicalaccess
{
    class MifarePlusACSACR1222L_SL1Commands : public virtual MifarePlusSL1PCSCCommands
    {
    public:
        virtual bool AESAuthenticate(std::shared_ptr<AES128Key> ptr,
                                     uint16_t keyslot) override;

    private:
      /**
       * This class setup a "Direct Transmit" channel to send
       * mifare plus commands directly to the card.
       *
       * The channel is setup in the constructor, and tore down
       * in the destructor.
       *
       * The ReaderCardAdapter from the command object is swapped out
       * by this one (and the old one is set when this object goes
       * out of scope)
       */
      class GenericSessionGuard
      {
        public:
          GenericSessionGuard(Commands *cmd);
          ~GenericSessionGuard();

        private:
          class Adapter : public ReaderCardAdapter
          {
              virtual std::vector<unsigned char>
              adaptCommand(const std::vector<unsigned char> &command) override;

              virtual std::vector<unsigned char>
              adaptAnswer(const std::vector<unsigned char> &answer) override;
          };

          /**
           * The "old" rca object.
           */
          std::shared_ptr<PCSCReaderCardAdapter> rca_;

          Commands *cmd_;

          std::shared_ptr<Adapter> adapter_;
      };
    };
}
