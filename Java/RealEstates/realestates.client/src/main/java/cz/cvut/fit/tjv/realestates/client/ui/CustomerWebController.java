package cz.cvut.fit.tjv.realestates.client.ui;

import cz.cvut.fit.tjv.realestates.client.model.CustomerDTO;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import cz.cvut.fit.tjv.realestates.client.data.CustomerClient;
import org.springframework.web.bind.annotation.*;

@Controller
@RequestMapping("rest/api/customer")
public class CustomerWebController {
    private final CustomerClient customerClient;

    public CustomerWebController(CustomerClient customerClient) {
        this.customerClient = customerClient;
    }

    @GetMapping(path = "/{id}")
    public String getCustomer(@PathVariable("id") String email, Model model) {
        model.addAttribute("customer", customerClient.getCustomerById(email));
        return "Customer/CustomerByEmail";
    }

    @GetMapping
    public String getCustomers(Model model) {
        model.addAttribute("customers", customerClient.getAllCustomers());
        model.addAttribute("newCustomer", new CustomerDTO());
        return "Customer/Customers";
    }

    @PostMapping
    public String registerCustomer(@ModelAttribute CustomerDTO customerDTO, Model model) {
        model.addAttribute(customerClient.createCustomer(customerDTO));
        return "redirect:/rest/api/customer";
    }

    @PostMapping("/{id}/update")
    public String updateCustomer(@PathVariable("id") String email, @ModelAttribute CustomerDTO customerDTO, Model model) {
        model.addAttribute("customerDTO", customerDTO);
        model.addAttribute("updateCustomer", customerClient.updateCustomer(customerDTO, email));
        return "redirect:/rest/api/customer";
    }

    @PostMapping("/{id}")
    public String deleteCustomer(@PathVariable("id") String email, Model model) {
        model.addAttribute(customerClient.deleteCustomerById(email));
        return "redirect:/rest/api/customer";
    }

    @PostMapping("/date")
    public String deleteCustomersByAge(Model model) {
        model.addAttribute(customerClient.deleteCustomersByAge());
        return "redirect:/rest/api/customer";
    }
}
