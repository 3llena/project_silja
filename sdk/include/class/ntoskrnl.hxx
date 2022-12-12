#pragma once

namespace sdk {
   struct ntoskrnl_t : kernel_module_t {
      [[ nodiscard ]]
      std::handle_t ps_create_system_thread(
         auto callback,
         auto argument
      ) {
         static auto call{ find_export( "PsCreateSystemThread" ) };
         if ( !call )
            return {};

         using call_t = std::int32_t(
            std::handle_t* handle,
            std::uint32_t access_mask,
            std::address_t attributes,
            std::handle_t ctx_process,
            std::address_t client_id,
            decltype( callback ),
            decltype( argument )
         );

         std::handle_t handle{};
         ptr< call_t* >( call )( &handle, 0, 0, 0, 0, callback, argument );
         return handle;
      }

      std::int8_t zw_close(
         std::handle_t handle
      ) {
         static auto call{ find_export( "ZwClose" ) };
         if ( !call )
            return 0;

         using func_t = std::int32_t( std::handle_t handle );
         return ptr< func_t* >( call )( handle ) == 0;
      }

      template< class... args_t >
      std::int8_t dbg_print(
         const char* string,
         args_t... va_args
      ) {
         static auto call{ find_export( "DbgPrintEx" ) };
         if ( !call )
            return 0;

         using func_t = std::int32_t(
            std::uint32_t flag,
            std::uint32_t level,
            const char* format,
            args_t... va_args
         );

         return ptr< func_t* >( call )( 0, 0, string, va_args... ) == 0;
      }

      std::int8_t ke_stack_attach_process(
         std::address_t process,
         kapc_state_t* apc_state
      ) {
         static auto call{ find_export( "KeStackAttachProcess" ) };
         if ( !call )
            return 0;

         using func_t = void(
            std::address_t process,
            kapc_state_t* apc_state
         );

         ptr< func_t* >( call )( process, apc_state );
         return 1;
      }

      std::int8_t ke_unstack_detach_process(
         kapc_state_t* apc_state
      ) {
         static auto call{ find_export( "KeUnstackDetachProcess" ) };
         if ( !call )
            return 0;

         using func_t = void(
            kapc_state_t* apc_state
         );

         ptr< func_t* >( call )( apc_state );
         return 1;
      }
   };
}